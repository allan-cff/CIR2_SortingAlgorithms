#include "algorithms.h"


void printArray(int *table, unsigned int len) {
    printf("[");
    for (unsigned int i = 0; i < len - 1; i++) {
        printf("%d,", table[i]);
    }
    printf("%d]", table[len - 1]);
}

int verif(int *table, unsigned int len) {
    unsigned int valid = 1;
    for (unsigned int i = 0; i < len - 1; i++) {
        if (table[i] > table[i + 1]) {
            valid = 0;
        }
    }
    return valid;
}

void printVerif(int *table, unsigned int len) {
    if (verif(table, len)) {
        printf("\nVerif tri complet OK !");
    } else {
        printf("\nERREUR tri incomplet !");
    }
}

double testAlgo(sortingAlgo tri, int *table, unsigned int len) {
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    (*tri)(table, len);
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (double) (end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

int *generateRandomArray(unsigned int len) {
    int *newTable;
    newTable = malloc(len * sizeof(int));
    if (newTable) {
        for (int i = 0; i < len; i++) {
            newTable[i] = rand() % 1000;
        }
    } else {
        printf("Malloc Error");
    }
    return newTable;
}

int *generatePowOfTenArray(unsigned int len, unsigned int power) {
    int *newTable;
    newTable = malloc(len * sizeof(int));
    if (newTable) {
        for (int i = 0; i < len; i++) {
            newTable[i] = pow(rand() % 10, rand() % power);
        }
    } else {
        printf("Malloc Error");
    }
    return newTable;
}

int *generateSortedArray(unsigned int len) {
    int *newTable;
    newTable = malloc(len * sizeof(int));
    if (newTable) {
        for (unsigned int i = 0; i < len; i++) {
            newTable[i] = i;
        }
    } else {
        printf("Malloc Error");
    }
    return newTable;
}

int *generateReversedArray(unsigned int len) {
    int *newTable;
    newTable = malloc(len * sizeof(int));
    if (newTable) {
        for (unsigned int i = 0; i < len; i++) {
            newTable[i] = len - i;
        }
    } else {
        printf("Malloc Error");
    }
    return newTable;
}

void testExecutionTime(sortingAlgo tri, unsigned int testNumbersPerLen, unsigned int shuffleSetup,
                       unsigned int firstArrayLen, unsigned int numberOfLenTested, unsigned int graphType,
                       unsigned int incrementation, FILE *outputPipe) {
    double time;
    int len = firstArrayLen;
    int *table;
    int power;
    int powIncrement;
    if (shuffleSetup == 4) {
        printf("\nEnter max power for array elements : ");
        scanf("%d", &power);
        printf("\nEnter power incrementation : ");
        scanf("%d", &powIncrement);
    }
    for (unsigned int i = 0; i < numberOfLenTested; i++) {
        time = 0;
        for (unsigned int j = 0; j < testNumbersPerLen; j++) {
            if (shuffleSetup == 1) {
                table = generateSortedArray(len);
            } else {
                if (shuffleSetup == 2) {
                    table = generateRandomArray(len);
                } else {
                    if (shuffleSetup == 3) {
                        table = generateReversedArray(len);
                    } else {
                        if (shuffleSetup == 4) {
                            table = generatePowOfTenArray(len, power);
                        }
                    }
                }
            }
            time += testAlgo(tri, table, len);
            free(table);
        }
        time = time / testNumbersPerLen;
        if(shuffleSetup == 4 && graphType == 0) {
            fprintf(outputPipe, "%d;%f\n", power, 1000 * time); // Sends point by point to outputStrean
            printf("%d;%f\n", power, 1000 * time);
            power += powIncrement;
        } else {
            fprintf(outputPipe, "%d;%f\n", len, 1000 * time); // Sends point by point to outputStrean
            printf("%d;%f\n", len, 1000 * time);
        }
        if (graphType == 1) {
            len += incrementation;
        } else if (graphType == 2) {
            len = len * incrementation;
        }
    }
}

void createPlot(sortingAlgo tri) {
    printf("\n==========          TESTS CONFIGURATION          ==========");
    unsigned int testNumbersPerLen;
    printf("\nEnter the number of tests to run for each array length (time result will be the average of all theses tests) :");
    scanf("%d", &testNumbersPerLen);
    unsigned int firstArrayLen;
    printf("\nEnter the start array length :");
    scanf("%d", &firstArrayLen);
    int graphType = -1;
    while (graphType != 0 && graphType != 1 && graphType != 2) {
        printf("\nChoose graph type :\n-0 : fixed size\n-1 : incrementation\n-2 : multiplicator\nYour choice : ");
        scanf("%d", &graphType);
    }
    unsigned int incrementation = 0;
    if (graphType == 1) {
        printf("\nEnter the array length incrementation between each tests :");
        scanf("%d", &incrementation);
    } else if (graphType == 2) {
        printf("\nEnter the array length multiplicator between each tests :");
        scanf("%d", &incrementation);
    }
    unsigned int numberOfLenTested = 0;
    printf("\nEnter the number of different array lengths (or array values range) to test :");
    scanf("%d", &numberOfLenTested);
    getchar();
    printf("\n==========          OUTPUT CONFIGURATION          ==========");
    unsigned int outputType = 0;
    while (outputType != 1 && outputType != 2) {
        printf("\nChoose output type :\n-1 : png file with graph\n-2 : csv file with points\nYour choice : ");
        scanf("%d", &outputType);
    }
    FILE *outputPipe;
    FILE *gnuplotPipe;
    char filename[32];
    char dataTitles[11][256];
    if (outputType == 1) {
        char logScaleAnswer = ' ';
        if (graphType == 2) {
            printf("\nYou chose multiplicator, do you want to print data with a log scale ? (y/n) : ");
            scanf("%c", &logScaleAnswer);
            getchar();
        }
        getchar();
        printf("\nEnter output file name (without extension) :");
        fgets(filename, 32, stdin);
        filename[strlen(filename) - 1] = '\0';
        char title[256];
        printf("\nEnter the title of the graph :");
        fgets(title, 256, stdin);
        title[strlen(title) - 1] = '\0';
        char *commandsForGnuplot[] = {
                "set datafile separator ';'",
                "set grid",
                "set xlabel 'number of elements'",
                "set ylabel 'time in milliseconds'",
        };
        gnuplotPipe = popen("gnuplot -persistent", "w"); //Opens a gnuplot terminal
        fprintf(gnuplotPipe, "set title '%s'\n", title);
        for (unsigned int i = 0; i < 4; i++) {
            fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Sends commands to gnuplot one by one.
        }
        if(logScaleAnswer == 'y' || logScaleAnswer == 'Y'){
            fprintf(gnuplotPipe, "set logscale x\n");
        } else {
            fprintf(gnuplotPipe, "set autoscale xy\n");
        }
    } else {
        getchar();
        printf("\nEnter output file name (with extension) :");
        fgets(filename, 32, stdin);
        filename[strlen(filename) - 1] = '\0';
        outputPipe = fopen(filename, "w");
    }
    unsigned int graphInputsNumber = 0;
    while (graphInputsNumber == 0 || graphInputsNumber > 10) {
        printf("\nEnter number of different input cases to test (max 10) : ");
        scanf("%d", &graphInputsNumber);
    }
    unsigned int shuffleSetup = 0;
    for (unsigned int i = 0; i < graphInputsNumber; i++) {
        while (shuffleSetup != 1 && shuffleSetup != 2 && shuffleSetup != 3 && shuffleSetup != 4) {
            printf("\nChoose enter array :\n-1 : ordered array\n-2 : shuffled array\n-3 : reverse sorted array\n-4 : power of ten array\nYour choice : ");
            scanf("%d", &shuffleSetup);
        }
        char temporaryFileName[12] = "temp";
        if (outputType == 1) {
            getchar();
            printf("\nEnter the name of this set of data :");
            fgets(dataTitles[i], 256, stdin);
            dataTitles[i][strlen(dataTitles[i]) - 1] = '\0';
            char temporaryFileNum[2] = "";
            sprintf(temporaryFileNum, "%d", i);
            strcat(temporaryFileName, temporaryFileNum);
            strcat(temporaryFileName, ".dat");
            outputPipe = fopen(temporaryFileName, "w");
        }
        testExecutionTime(tri, testNumbersPerLen, shuffleSetup, firstArrayLen, numberOfLenTested, graphType,
                          incrementation, outputPipe);
        if (outputType == 1) {
            fclose(outputPipe);
            int tendance = -1;
            while (tendance != 0 && tendance != 1 && tendance != 2 && tendance != 3) {
                printf("\nChoose tendance for data set %s :\n-0 none\n-1 n\n-2 n**2\n-3 nlog(n)\nYour choice : ", dataTitles[i]);
                scanf("%d", &tendance);
            }
            if (tendance != 0) {
                if (tendance == 1) {
                    fprintf(gnuplotPipe, "f%d(x) = c * x\n", i);
                    fprintf(gnuplotPipe, "title_f%d(c) = sprintf(\"f(x) = c*n, c = %s\", c)\n", i, "%f");
                } else if (tendance == 2) {
                    fprintf(gnuplotPipe, "f%d(x) = c * x ** 2\n", i);
                    fprintf(gnuplotPipe, "title_f%d(c) = sprintf(\"f(x) = c*n^2, c = %s\", c)\n", i, "%f");
                } else {
                    fprintf(gnuplotPipe, "f%d(x) = c * x * log(x)\n", i);
                    fprintf(gnuplotPipe, "title_f%d(c) = sprintf(\"f(x) = c*nlog(n), c = %s\", c)\n", i, "%f");
                }
                fprintf(gnuplotPipe, "fit f%d(x) '%s' via c\n", i, temporaryFileName);
                fprintf(gnuplotPipe, "%s '%s' title '%s', f%d(x) title title_f%d(c) with lines\n", i == 0 ? "plot" : "replot", temporaryFileName, dataTitles[i], i, i);
                printf("%s '%s' title '%s', f%d(x) title title_f%d(c) with lines\n", i == 0 ? "plot" : "replot", temporaryFileName, dataTitles[i], i, i);
            } else {
                fprintf(gnuplotPipe, "%s '%s' title '%s'\n", i == 0 ? "plot" : "replot", temporaryFileName, dataTitles[i]);
                printf("%s '%s' title '%s'\n", i == 0 ? "plot" : "replot", temporaryFileName, dataTitles[i]);
            }
        }
        shuffleSetup = 0;
    }
    if (outputType == 1) {
        fprintf(gnuplotPipe, "set terminal png\n");
        fprintf(gnuplotPipe, "set output '%s.png'\n", filename);
        fprintf(gnuplotPipe, "replot\n");
        int exit = pclose(gnuplotPipe); // Exits the stream
        if (exit == 0) {
            printf("Image successfully created !");
            for(unsigned int i = 0; i < graphInputsNumber; i++) {
                char temporaryFileName[12] = "temp";
                char temporaryFileNum[2] = "";
                sprintf(temporaryFileNum, "%d", i);
                strcat(temporaryFileName, temporaryFileNum);
                strcat(temporaryFileName, ".dat");
                remove(temporaryFileName);
            }
        } else {
            printf("GnuPlot ERROR exited with code %d", exit);
        }
    } else {
        fclose(outputPipe);
    }
}

void selectionSort(int *table, unsigned int len) {
    int minIndex;
    int save;
    for (unsigned int i = 0; i < len; i++) {
        minIndex = i;
        for (unsigned int j = i + 1; j < len; j++) {
            if (table[minIndex] > table[j]) {
                minIndex = j;
            }
        }
        save = table[i];
        table[i] = table[minIndex];
        table[minIndex] = save;
    }
}

void insertionSort(int *array, unsigned int len) {
    int save, j;
    for (int i = 1; i < len; i++) {
        save = array[i];
        j = i - 1;
        while (j >= 0 && array[j] > save) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = save;
    }
}

void bubbleSort(int *array, unsigned int len) {
    unsigned int verif = 0;
    for (unsigned int i = 0; i < len - 1; i++) {
        verif = 0;
        for (unsigned int j = 0; j < len - i - 1; j++) {
            if (array[j + 1] < array[j]) {
                int save = array[j + 1];
                array[j + 1] = array[j];
                array[j] = save;
                verif = 1;
            }
        }
        if (verif == 0) {
            break;
        }
    }
}

void quickSort(int *table, unsigned int len) {

}

void mergeSort(int *table, unsigned int len) {

}

void heapSort(int *table, unsigned int len) {
    for (int i = (len - 1) / 2; i >= 0; i--) {
        nodeSieving(table, len, i);
    }
    unsigned int initialLen = len;
    for (unsigned int j = 0; j < initialLen - 1; j++) {
        len--;
        int save = table[0];
        table[0] = table[len];
        table[len] = save;
        for (int k = 0; k <= (len - 1) / 2; k++) {
            nodeSieving(table, len, k);
        }
    }
}


void nodeSieving(int *table, unsigned int len, unsigned int cursor) {
    unsigned int left = cursor * 2 + 1;
    unsigned int right = (cursor + 1) * 2;
    unsigned int maxpos = left;
    if (left < len) {
        if (right < len) {
            if (table[right] > table[left]) {
                maxpos = right;
            }
        }
        if (table[maxpos] > table[cursor]) {
            int save = table[maxpos];
            table[maxpos] = table[cursor];
            table[cursor] = save;
            nodeSieving(table, len, maxpos);
        }
    }
}

void countingSort(int *array, unsigned int size, unsigned int power) {
    int output[size + 1];
    int count[10];
    for (int i = 0; i < 10; ++i) {
        count[i] = 0;
    }
    for (int i = 0; i < size; i++) {
        count[(array[i] / power) % 10]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = size - 1; i >= 0; i--) {
        output[count[(array[i] / power) % 10] - 1] = array[i];
        count[(array[i] / power) % 10]--;
    }

    for (int i = 0; i < size; i++) {
        array[i] = output[i];
    }
}

void radixSort(int *arr, unsigned int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    for (unsigned int power = 1; max / power > 0; power *= 10) {
        countingSort(arr, size, power);
    }
}

void bogoSort(int *array, unsigned int len) {
    while (!verif(array, len)) {
        for (unsigned int i = 0; i < len; i++) {
            int j = rand() % (len);
            int save = array[i];
            array[i] = array[j];
            array[j] = save;
        }
    }
}
