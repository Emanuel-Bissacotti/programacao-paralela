#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size, n;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Digite o tamanho da matriz (n x n): ");
        scanf("%d", &n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = n / size;
    int extra = n % size;

    int start_row = rank * rows_per_proc + (rank < extra ? rank : extra);
    int local_rows = rows_per_proc + (rank < extra ? 1 : 0);

    double* matrix = (double*)malloc(local_rows * n * sizeof(double));
    double* vector = (double*)malloc(n * sizeof(double));
    double* result_local = (double*)malloc(local_rows * sizeof(double));

    srand(rank + 7);
    for (int i = 0; i < local_rows * n; i++)
        matrix[i] = rand() % 10;

    if (rank == 0) {
        for (int i = 0; i < n; i++)
            vector[i] = rand() % 10;
    }

    MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    for (int i = 0; i < local_rows; i++) {
        result_local[i] = 0;
        for (int j = 0; j < n; j++)
            result_local[i] += matrix[i * n + j] * vector[j];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
        printf("\nTempo total: %f segundos\n", end - start);

    free(matrix);
    free(vector);
    free(result_local);

    MPI_Finalize();
    return 0;
}
