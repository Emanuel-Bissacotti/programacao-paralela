#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void inicializar_matriz(double **matriz, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = (double)rand() / RAND_MAX * 100.0;
        }
    }
}

double** alocar_matriz(int n) {
    double **matriz = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        matriz[i] = (double*)malloc(n * sizeof(double));
    }
    return matriz;
}

void liberar_matriz(double **matriz, int n) {
    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void multiplicacao_paralela(double **A, double **B, double **C, int n) {
    #pragma omp parallel for shared(A, B, C, n) 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void verificar_resultado(double **C, int n) {
    double soma = 0.0;
    #pragma omp parallel for reduction(+:soma)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            soma += C[i][j];
        }
    }
    printf("Soma dos elementos da matriz resultado: %.2f\n", soma);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <tamanho_da_matriz> <numero_de_threads>\n", argv[0]);
        return -1;
    }
    
    int n = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    double tempo_inicio, tempo_fim, tempo_total;

    omp_set_num_threads(num_threads);
    
    srand(time(NULL));
    
    double **A = alocar_matriz(n);
    double **B = alocar_matriz(n);
    double **C = alocar_matriz(n);
    
    inicializar_matriz(A, n);
    inicializar_matriz(B, n);
    
    tempo_inicio = get_time();
    
    multiplicacao_paralela(A, B, C, n);
    
    tempo_fim = get_time();
    tempo_total = tempo_fim - tempo_inicio;

    if (n <= 10) {
        verificar_resultado(C, n);
    }

    FILE *arquivo = fopen("resultado_paralelo.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d,%d,%.6f\n", n, num_threads, tempo_total);
        fclose(arquivo);
    }
    liberar_matriz(A, n);
    liberar_matriz(B, n);
    liberar_matriz(C, n);
    
    return 0;
}