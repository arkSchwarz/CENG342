#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hellomake.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s rows columns output_file\n", argv[0]);
        return 1;
    }

    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    char *output = argv[3];

    double **matrix = (double **) malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *) calloc(columns, sizeof(double));
    }

   long int i = 18050111041;
    srand(i);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = (double) rand() / RAND_MAX * 99.0 + 1.0;
        }
    }

    double *vector = (double *) calloc(columns, sizeof(double));
    for (int i = 0; i < columns; i++) {
        vector[i] = (double) rand() / RAND_MAX * 99.0 + 1.0;
    }

    double *result = (double *) calloc(rows, sizeof(double));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    FILE *file = fopen(output, "w");
    for (int i = 0; i < rows; i++) {
        fprintf(file, "%lf\n", result[i]);
    }
    fclose(file);

    free(matrix);
    free(vector);
    free(result);
    
     myPrintHelloMake();

    return 0;
}


