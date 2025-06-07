#include "givenA1.h" 
#include <stdio.h>
#include <math.h>

/*
 * This file includes:
 *  - readFromFile: reads the zoo dataset into dataZoo
 *  - distanceFunctions: computes Euclidean, Hamming, Jaccard
 *  - findKNearestNeighbors: finds k nearest neighbors for a given sample
 *  - quickSort: sorts float arrays (distances) and keeps track of indices
 *  - predictClass: returns the most frequent class among k neighbors
 *  - findAccuracy: compares predicted classes to actual labels in testData
 */

void quickSort(float arr[], int indexArr[], int low, int high, int ascending);

int readFromFile(char fName[30], struct Animal dataZoo[NUM_SAMPLES]) {
    int i, j, k;

    FILE *file = fopen(fName, "r");
    if (file == NULL) {
        printf("Error opening file");
        return -1;
    }

    // Read each row (total NUM_SAMPLES), 18 columns: 1 name + 16 features + 1 class label
    for (i = 0; i < NUM_SAMPLES; i++) {
        for (j = 0; j < 18; j++) {
            if (j == 0) {
                fscanf(file, "%s", dataZoo[i].animalName); 
            }
            else if (j == 17) {
                fscanf(file, "%d", &dataZoo[i].classLabel);
            }
            else {
                fscanf(file, "%d", &dataZoo[i].features[j-1]);
            }
        }

        // Print each animal and their details (name, features and class label)
        printf("%s ", dataZoo[i].animalName);
        for (k = 0; k < NUM_FEATURES; k++) {
            printf("%d ", dataZoo[i].features[k]);
        }
        printf("%d\n", dataZoo[i].classLabel);
    }
    return 1;
}

void distanceFunctions(int vector1[NUM_FEATURES],
                       int vector2[NUM_FEATURES],
                       float *euclideanDistance,
                       int *hammingDistance,
                       float *jaccardSimilarity)
{
    int i;
    int hammingCount = 0;
    float jaccardCount1 = 0; // Count positions where both are 1
    float jaccardCount0 = 0; // Count positions where both are 0
    float euclideanCount = 0;

    // Hamming distance: number of differing positions
    for (i = 0; i < NUM_FEATURES; i++) {
        if (vector1[i] != vector2[i]) {
            hammingCount++;
        }
    }
    *hammingDistance = hammingCount;

    // Euclidean Distance: sqrt( sum( (v1[i] - v2[i])^2 ) )
    for (i = 0; i < NUM_FEATURES; i++) {
        euclideanCount += pow((vector1[i] - vector2[i]), 2);
    }
    euclideanCount = pow(euclideanCount, 0.5);
    *euclideanDistance = euclideanCount;

    // Jaccard Similarity: # positions (1 & 1) / (totalFeatures - # positions (0 & 0))
    for (i = 0; i < NUM_FEATURES; i++) {
        if (vector1[i] == 1 && vector2[i] == 1) {
            jaccardCount1++;
        }
    }
    for (i = 0; i < NUM_FEATURES; i++) {
        if (vector1[i] == 0 && vector2[i] == 0) {
            jaccardCount0++;
        }
    }
    *jaccardSimilarity = jaccardCount1 / (NUM_FEATURES - jaccardCount0);
}

void findKNearestNeighbors(struct Animal dataZoo[NUM_SAMPLES],
                           int newSample[NUM_FEATURES],
                           int k,
                           int whichDistanceFunction,
                           int kNearestNeighbors[NUM_SAMPLES])
{
    // Pointers for distanceFunctions
    float eucDist = 0;
    float *eucDistPtr = &eucDist;
    int hamDist = 0;
    int *hamDistPtr = &hamDist;
    float jacDist = 0;
    float *jacDistPtr = &jacDist;

    // Distances, indices
    float tempList[3];
    float distList[NUM_SAMPLES];
    int indexList[NUM_SAMPLES];

    // Determine sort order: ascending (1) for Euclidiean/Hamming, descending (0) for Jaccard
    int sortOrder = 1;
    int n = 1;

    for (int i = 0; i < NUM_SAMPLES; i++) {
        // Compute all 3 distances for each data row
        distanceFunctions(newSample, dataZoo[i].features, eucDistPtr, hamDistPtr, jacDistPtr);

        tempList[0] = eucDist;  // Euclidean
        tempList[1] = hamDist;  // Hamming (cast int to float if needed)
        tempList[2] = jacDist;  // Jaccard
        distList[i] = tempList[whichDistanceFunction - 1];
        indexList[i] = i;
    }

    if (whichDistanceFunction == 3) {
        // For Jaccard, we want descending sort
        sortOrder = 0;
    }

    // Sort distances and indices accordingly
    quickSort(distList, indexList, 0, NUM_SAMPLES - 1, sortOrder);

    // Copy the top k indices into kNearestNeighbors
    for (int i = 0; i < k; i++) {
        kNearestNeighbors[i] = indexList[i];
    }
}

// Quicksort
// Swaps for float and int arrays
void swap(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void swapInt(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function supporting ascending or descending
int partition(float arr[], int indexArr[], int low, int high, int ascending) {
    float pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        // If ascending, arrange smaller values first.
        // If descending, arrange larger values first.
        if ((ascending && arr[j] < pivot) || (!ascending && arr[j] >= pivot)) {
            i++;
            swap(&arr[i], &arr[j]);
            swapInt(&indexArr[i], &indexArr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    swapInt(&indexArr[i + 1], &indexArr[high]);
    return i + 1;
}

void quickSort(float arr[], int indexArr[], int low, int high, int ascending) {
    if (low < high) {
        int pi = partition(arr, indexArr, low, high, ascending);
        quickSort(arr, indexArr, low, pi - 1, ascending);
        quickSort(arr, indexArr, pi + 1, high, ascending);
    }
}

// Predicts the class by taking the most frequent among the k-nearest neighbors
int predictClass(struct Animal dataZoo[NUM_SAMPLES],
                 int newSample[NUM_FEATURES],
                 int whichDistanceFunction,
                 int k)
{
    int i;
    int kNearestNeighbors[NUM_SAMPLES];

    // Find the k nearest neighbors
    findKNearestNeighbors(dataZoo, newSample, k, whichDistanceFunction, kNearestNeighbors);

    // Count occurrences of each class among those k neighbors
    int kNearNeighborsClasses[k];
    int classCounts[NUM_CLASSES + 1] = {0};
    int highestClassCount = 0;
    int classValToReturn = 1;

    // Gather the classes of the k neighbors
    for (i = 0; i < k; i++) {
        kNearNeighborsClasses[i] = dataZoo[kNearestNeighbors[i]].classLabel;
    }

    // Count how often each class label appears
    for (i = 0; i < k; i++) {
        classCounts[kNearNeighborsClasses[i]]++;
    }

    // Find the class with the highest frequency; in a tie, smaller class label wins
    highestClassCount = classCounts[1];
    for (i = 2; i < NUM_CLASSES + 1; i++) {
        if (classCounts[i] > highestClassCount) {
            highestClassCount = classCounts[i];
            classValToReturn = i;
        }
    }
    return classValToReturn;
}

// Computes accuracy by comparing predictions on testData with their actual labels
float findAccuracy(struct Animal dataZoo[NUM_SAMPLES],
                   int whichDistanceFunction,
                   struct Animal testData[NUM_TEST_DATA],
                   int k)
{
    float accuracy;
    float numCorrectPredictions = 0;
    int tempPrediction;

    for (int i = 0; i < NUM_TEST_DATA; i++) {
        tempPrediction = predictClass(dataZoo, testData[i].features, whichDistanceFunction, k);
        if (tempPrediction == testData[i].classLabel) {
            numCorrectPredictions++;
        }
    }

    accuracy = numCorrectPredictions / NUM_TEST_DATA;
    return accuracy;
}
