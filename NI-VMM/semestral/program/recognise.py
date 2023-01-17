import helper as hp
import sys


# Constants, global variables
DATABASE_FILE = "db.dat"
dataset = hp.loadDataset(DATABASE_FILE)


# Function to recognise music genre of given file
def recognise(file, k_nearest, dtw_width, descriptor_weights):
    feature = hp.getFeatures(file)
    feature.append('')
    nearest_class = hp.nearestClass(hp.getNeighbours(
        dataset, feature, descriptor_weights, dtw_width, k_nearest
    ))
    return nearest_class


# Example of usage of the function
if __name__ == '__main__':
    if len(sys.argv) != 11:
        print("Usage: python3 recognise.py <file> <k_nearest> <dtw_width> "
              "<spectrum_spread_weight> <spectral_centroid_weight> "
              "<area_of_moments_weight> <area_of_moments_std_weight> <moments_weight>"
              "<mfcc_mean_weight> <mfcc_weight>")
        exit(1)

    print(recognise(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]), [
        float(sys.argv[4]), float(sys.argv[5]), float(sys.argv[6]), float(sys.argv[7]), float(sys.argv[8]), float(sys.argv[9]), float(sys.argv[10])
    ]))
