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

    int total_elems = n * n;
    int elems_per_proc = total_elems / size;
    int extra = total_elems % size;

    int start_idx = rank * elems_per_proc + (rank < extra ? rank : extra);
    int local_elems = elems_per_proc + (rank < extra ? 1 : 0);

    double* matrix = (double*)malloc(local_elems * sizeof(double));
    srand(rank + 5);
    for (int i = 0; i < local_elems; i++)
        matrix[i] = rand() % 1000;

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    double local_max = matrix[0];
    for (int i = 1; i < local_elems; i++)
        if (matrix[i] > local_max)
            local_max = matrix[i];

    double global_max;
    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0) {
        printf("\nMaior valor encontrado: %.2f\n", global_max);
        printf("Tempo total: %f segundos\n", end - start);
    }

    free(matrix);
    MPI_Finalize();
    return 0;
}
