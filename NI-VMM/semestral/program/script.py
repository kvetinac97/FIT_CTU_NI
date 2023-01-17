import helper as hp
import os
import pickle
import random


# File path constants
DATABASE_FILE = "db.dat"
AUDIO_PATH = "../audio/"

# Accuracy testing constants
SPLIT_PROBABILITY = 0.6
TEST_K_NEAREST = 5
DTW_WIDTH = 30
WEIGHTS = [1, 1, 1, 1, 1, 1, 0]


# Generates database with clusters to recognise audio genre
def buildDatabase():
    file = open(DATABASE_FILE, 'wb')  # create DB file
    for genre in os.listdir(AUDIO_PATH):
        for audioFile in os.listdir(AUDIO_PATH + genre):
            audioFilePath = AUDIO_PATH + genre + '/' + audioFile
            feature = hp.getFeatures(audioFilePath)
            feature.append(genre)
            pickle.dump(feature, file)
    file.flush()
    file.close()


# Helper function to load
def loadDataset(training_set, test_set):
    dataset = hp.loadDataset(DATABASE_FILE)
    dataset_length = len(dataset)
    for dataset_idx in range(dataset_length):
        if dataset_idx % 2 == 0:
            training_set.append(dataset[dataset_idx])
        else:
            test_set.append(dataset[dataset_idx])


# Tests precision
def testPrecision():
    # Load training and test set
    training_set = []
    test_set = []
    loadDataset(training_set, test_set)

    # Make predictions
    test_length = len(test_set)
    predictions = []
    for test_idx in range(test_length):
        predictions.append(hp.nearestClass(hp.getNeighbours(training_set, test_set[test_idx],
                                           WEIGHTS, DTW_WIDTH, TEST_K_NEAREST)))

    print("Test set: ", list(map(lambda x: x[-1], test_set)))
    print("Predictions: ", list(map(lambda x: x[0][0], predictions)))

    # Calculate accuracy
    correct_tries = 0
    for test_idx in range(test_length):
        if test_set[test_idx][-1] == predictions[test_idx][0][0]:
            correct_tries += 1
    return 100.0 * correct_tries / test_length


if __name__ == '__main__':
    print("Building database...")
    buildDatabase()
    print("Calculating precision...")
    precision = testPrecision()
    print(f"Precision: {precision} %")
