#ifndef SORTING_ALGORITHMS_H
#define SORTING_ALGORITHMS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef void (*sortingAlgo)(int*, unsigned int); //We define a new type that contains a pointer to function

void printArray(int*, unsigned int);
int verif(int*, unsigned int);
void printVerif(int*, unsigned int);
double testAlgo(sortingAlgo, int* array, unsigned int length);  //Prends une fonction d'algo de tri et un échantillon et fait les tests
void swap(int* x, int* y);
int* generateRandomArray(unsigned int len);
int* generateSortedArray(unsigned int length);
int *generatePowOfTenArray(unsigned int len, unsigned int pow);
int* generateReversedArray(unsigned int length);
void testExecutionTime(sortingAlgo tri, unsigned int testNumbersPerLen, unsigned int shuffleTimes, unsigned int firstArrayLen, unsigned int numberOfLenTested, unsigned int graphType, unsigned int incrementation, FILE* outputPipe);
void createPlot(sortingAlgo tri);

void selectionSort(int*, unsigned int);
void insertionSort(int*, unsigned int);
void bubbleSort(int*, unsigned int);
void quickSort(int*, unsigned int);
void mergeSort(int*, unsigned int);
void heapSort(int*, unsigned int);
void nodeSieving(int *table, unsigned int len, unsigned int cursor);
void countingSort(int*, unsigned int, unsigned int);
void radixSort(int*, unsigned int);
void bogoSort(int*, unsigned int);

#endif //SORTING_ALGORITHMS_H