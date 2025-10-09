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

    double* A = (double*)malloc(n * local_rows * sizeof(double));
    double* B = (double*)malloc(n * local_rows * sizeof(double));
    double* C = (double*)malloc(n * local_rows * sizeof(double));

    srand(rank + 1);
    for (int i = 0; i < n * local_rows; i++) {
        A[i] = rand() % 10;
        B[i] = rand() % 10;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    for (int i = 0; i < n * local_rows; i++)
        C[i] = A[i] + B[i];

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0)
        printf("\nTempo total: %f segundos\n", end - start);

    free(A);
    free(B);
    free(C);

    MPI_Finalize();
    return 0;
}
