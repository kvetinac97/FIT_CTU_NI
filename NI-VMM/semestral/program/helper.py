import loader as ld
import numpy as np
import operator
import pickle


# Function to measure distance of two arrays
# Uses Dynamic Time Warping algorithm
def distance_dtw(first_arr, second_arr, dtw_width):
    # Create DTW matrix
    first_len = len(first_arr)
    second_len = len(second_arr)
    dtw_matrix = np.zeros((first_len + 1, second_len + 1))

    width = max(dtw_width, abs(first_len - second_len))

    # Pre-fill distance with Infinity (max)
    for i in range(1, first_len + 1):
        for j in range(1, second_len + 1):
            dtw_matrix[i][j] = np.Infinity

    # The DTW algorithm
    dtw_matrix[0][0] = 0
    for i in range(1, first_len + 1):
        for j in range(max(1, i - width), min(second_len, i + width) + 1):
            dtw_matrix[i][j] = 0
    for i in range(1, first_len + 1):
        for j in range(max(1, i - width), min(second_len, i + width) + 1):
            cost = abs(first_arr[i - 1] - second_arr[j - 1])
            dtw_matrix[i][j] = cost + np.min([
                dtw_matrix[i - 1][j],
                dtw_matrix[i][j - 1],
                dtw_matrix[i - 1][j - 1]
            ])

    # Resulting distance
    return dtw_matrix[first_len][second_len]


# Function to compute general distance of two arrays of descriptors
# third array describes weight of each descriptor
# array length must be the same
def distance(instance1, instance2, weights, dtw_width):
    instance1_length = len(instance1)
    instance2_length = len(instance2)
    weights_length = len(weights)

    if instance1_length != instance2_length or instance2_length != weights_length + 1:
        print(f"Instance length mismatch! {instance1_length} {instance2_length} {weights_length+1}")
        return np.Infinity

    final_distance = 0
    for descriptor_index in range(instance1_length - 1):
        if weights[descriptor_index] == 0:
            continue
        final_distance += weights[descriptor_index] * distance_dtw(
            instance1[descriptor_index],
            instance2[descriptor_index],
            dtw_width
        )
    return final_distance


# Finds k nearest neighbours of given instance
def getNeighbours(dataset, instance, weights, dtw_width, k):
    # Calculate each distance
    distance_array = []
    dataset_length = len(dataset)
    for instance_idx in range(dataset_length):
        curr_instance = dataset[instance_idx]
        dist = distance(curr_instance, instance, weights, dtw_width)
        dist += distance(instance, curr_instance, weights, dtw_width)
        distance_array.append((curr_instance[-1], dist))

    # Find nearest neighbours
    distance_array.sort(key=operator.itemgetter(1))
    neighbours = []
    for neighbour_index in range(k):
        neighbours.append(distance_array[neighbour_index][0])
    return neighbours


# Returns probability of genres in neighbours
def nearestClass(neighbours):
    classVote = {}
    neighbours_length = len(neighbours)
    for neighbour_index in range(neighbours_length):
        response = neighbours[neighbour_index]
        if response in classVote:
            classVote[response] += (1 / neighbours_length)
        else:
            classVote[response] = (1 / neighbours_length)
    sorter = sorted(classVote.items(), key=operator.itemgetter(1), reverse=True)
    return sorter


# Loads dataset from given file
def loadDataset(filename):
    dataset = []
    with open(filename, 'rb') as file:
        while True:
            try:
                dataset.append(pickle.load(file))
            except EOFError:
                file.close()
                break
    return dataset


# Load features from given audio file
def getFeatures(file):
    print(f"Loading features for {file}")

    # MPEG7 descriptors
    centroid_average, spread_average = ld.loadMpegDescriptors(file)
    spectrum_spread = [200 * spread_average]
    spectral_centroid = [100 * centroid_average]

    # jAudio descriptors
    area_of_moments, area_of_moments_std_dev, moments = ld.loadJAudioDescriptors(file)
    area_of_moments = [area_of_moments / 76868200000]
    area_of_moments_std_dev = [area_of_moments_std_dev / 116418200000]
    moments = [moments / 86000]

    # Python descriptors
    mfcc_mean, mfcc_time = ld.loadMfccCoefficients(file)

    return [spectrum_spread, spectral_centroid, area_of_moments, area_of_moments_std_dev, moments, mfcc_mean, mfcc_time]
