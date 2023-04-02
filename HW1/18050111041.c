#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    char *output = argv[3];

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = rows / size;
    double **matrix = (double **) malloc(rows_per_proc * sizeof(double *));
    for (int i = 0; i < rows_per_proc; i++) {
        matrix[i] = (double *) calloc(columns, sizeof(double));
    }

    long int i = 18050111041 + rank;
    srand(i);
    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = (double) rand() / RAND_MAX * 99.0 + 1.0;
        }
    }

    double *vector = (double *) calloc(columns, sizeof(double));
    for (int i = 0; i < columns; i++) {
        vector[i] = (double) rand() / RAND_MAX * 99.0 + 1.0;
    }

    double *result = (double *) calloc(rows_per_proc, sizeof(double));
    double start_time = MPI_Wtime();
    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < columns; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    double end_time = MPI_Wtime();

    double local_time = end_time - start_time;
    double global_time;
    MPI_Reduce(&local_time, &global_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    double speedup = global_time / local_time;
    double efficiency = speedup / size;

    double *gathered_result = NULL;
    if (rank == 0) {
        gathered_result = (double *) calloc(rows, sizeof(double));
    }

    MPI_Gather(result, rows_per_proc, MPI_DOUBLE, gathered_result, rows_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        FILE *file = fopen(output, "w");
        for (int i = 0; i < rows; i++) {
            fprintf(file, "%lf\n", gathered_result[i]);
        }
        fclose(file);
        printf("Global Time: %lf\n", global_time);
        printf("Speedup: %lf\n", speedup);
        printf("Efficiency: %lf\n", efficiency);
        free(gathered_result);
    }

    free(matrix);
    free(vector);
    free(result);

    MPI_Finalize();

    return 0;
}
