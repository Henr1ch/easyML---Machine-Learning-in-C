#include <stdio.h>
#include "givenA1.h"
#include <math.h>

/*
 * Main program that:
 *  1) Reads dataset from file (must do first).
 *  2) Demonstrates distance functions.
 *  3) Finds k-nearest neighbors.
 *  4) Predicts class for a sample.
 *  5) Finds accuracy using test data.
 */

int main(int argc, char * argv[]) {

    // Array to store the full dataset of animals
    struct Animal dataZoo[NUM_SAMPLES];

    // File name is taken from the command line argument
    char *file = argv[1];

    // Example newSample to test k-NN and class prediction
    int newSample[NUM_FEATURES] = {1,1,1,0,1,0,1,1,1,0,0,0,0,1,1,1};

    // Array to hold the indices of k-nearest neighbors
    int kNearestNeighbors[NUM_SAMPLES];

    // Two example vectors to demo distanceFunctions
    int vector1[NUM_FEATURES] = {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1};
    int vector2[NUM_FEATURES] = {1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 4, 0, 0, 1};

    // Distance metrics placeholders
    float eucDistance = 0;
    int hamDistance = 0;
    float jacSimilarity = 0;

    // Pointers to the above variables (used by distanceFunctions)
    float * eucPtr = &eucDistance;
    int * hamPtr = &hamDistance;
    float * jacPtr = &jacSimilarity;

    // Will hold an accuracy result for task 5
    float accuracy;

    // Flag to ensure we read the file first before other tasks
    int didOne = 0;

    // Menu choice
    int choice;

    // Class prediction result
    int classPrediction;

    // Loop counters / general use
    int i;
    int k = 5;

    // Some example test data to demonstrate Task 5 (accuracy):
    struct Animal testData[NUM_TEST_DATA] = {
        {"calf", {1,0,0,1,0,0,0,1,1,1,0,0,4,1,1,1}, 1},
        {"cheetah", {1,0,0,1,0,0,1,1,1,1,0,0,4,1,0,1}, 1},
        {"crab", {0,0,1,0,0,1,1,0,0,0,0,0,4,0,0,0}, 7},
        {"dogfish", {0,0,1,0,0,1,1,1,1,0,0,1,0,1,0,1}, 4},
        {"elephant", {1,0,0,1,0,0,0,1,1,1,0,0,4,1,0,1}, 1},
        {"flamingo", {0,1,1,0,1,0,0,0,1,1,0,0,2,1,0,1}, 2},
        {"fruitbat", {1,0,0,1,1,0,0,1,1,1,0,0,2,1,0,0}, 1},
        {"gnat", {0,0,1,0,1,0,0,0,0,1,0,0,6,0,0,0}, 6},
        {"gorilla", {1,0,0,1,0,0,0,1,1,1,0,0,2,0,0,1}, 1},
        {"haddock", {0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,0}, 4},
        {"sealion", {1,0,0,1,0,1,1,1,1,1,0,1,2,1,0,1}, 1},
        {"seasnake", {0,0,0,0,0,1,1,1,1,0,1,0,0,1,0,0}, 3},
        {"seawasp", {0,0,1,0,0,1,1,0,0,0,1,0,0,0,0,0}, 7},
        {"skimmer", {0,1,1,0,1,1,1,0,1,1,0,0,2,1,0,0}, 2},
        {"skua", {0,1,1,0,1,1,1,0,1,1,0,0,2,1,0,0}, 2},
        {"slowworm", {0,0,1,0,0,0,1,1,1,1,0,0,0,1,0,0}, 3},
        {"slug", {0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0}, 7},
        {"sole", {0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,0}, 4},
        {"sparrow", {0,1,1,0,1,0,0,0,1,1,0,0,2,1,0,0}, 2},
        {"squirrel", {1,0,0,1,0,0,0,1,1,1,0,0,2,1,0,0}, 1}
    };

    // Main menu loop
    while (1) {
        printf("\n=== Main Menu ===\n");
        printf("1) Read from file (MUST do first)\n");
        printf("2) Distance Functions\n");
        printf("3) Find K Nearest Neighbors\n");
        printf("4) Predict Class\n");
        printf("5) Find Accuracy\n");
        printf("Enter any other number to exit.\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            didOne = 1;  // We successfully read the file
            readFromFile(file, dataZoo);
        }
        else if (didOne != 1) {
            // If user tries to do tasks 2-5 before reading file
            printf("Please choose 1 first, program is ending!\n");
            break;
        }
        else if (choice == 2) {
            // Demonstrate distanceFunctions using vector1 and vector2
            distanceFunctions(vector1, vector2, eucPtr, hamPtr, jacPtr);
            printf("Hamming Distance: %d\nEuclidean Distance: %.2f\nJaccard Similarity: %.2f\n",
                   hamDistance, eucDistance, jacSimilarity);
        }
        else if (choice == 3) {
            // Show nearest neighbors for each distance function
            findKNearestNeighbors(dataZoo, newSample, k, 1, kNearestNeighbors);
            printf("Nearest neighbors for new sample with Euclidean distance: ");
            for (i = 0; i < k; i++) {
                printf("%d ", kNearestNeighbors[i]);
            }
            printf("\n");

            findKNearestNeighbors(dataZoo, newSample, k, 2, kNearestNeighbors);
            printf("Nearest neighbors for new sample with Hamming distance: ");
            for (i = 0; i < k; i++) {
                printf("%d ", kNearestNeighbors[i]);
            }
            printf("\n");

            findKNearestNeighbors(dataZoo, newSample, k, 3, kNearestNeighbors);
            printf("Nearest neighbors for new sample with Jaccard similarity: ");
            for (i = 0; i < k; i++) {
                printf("%d ", kNearestNeighbors[i]);
            }
            printf("\n");
        }
        else if (choice == 4) {
            // Predict class for the newSample under each distance function
            classPrediction = predictClass(dataZoo, newSample, 1, 4);
            printf("Prediction: %d\n", classPrediction);

            classPrediction = predictClass(dataZoo, newSample, 2, 4);
            printf("Prediction: %d\n", classPrediction);

            classPrediction = predictClass(dataZoo, newSample, 3, 4);
            printf("Prediction: %d\n", classPrediction);
        }
        else if (choice == 5) {
            // Compute accuracy for each distance function using testData
            accuracy = findAccuracy(dataZoo, 1, testData, k);
            printf("Accuracy for Euclidean distance: %f\n", accuracy);

            accuracy = findAccuracy(dataZoo, 2, testData, k);
            printf("Accuracy for Hamming distance: %f\n", accuracy);

            accuracy = findAccuracy(dataZoo, 3, testData, k);
            printf("Accuracy for Jaccard similarity: %f\n", accuracy);
        }
        else {
            // Exiting the program if choice is outside 1 to 5
            printf("Program is ending!\n");
            break;
        }
    }

    return 0;
}
