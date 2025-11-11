#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void inicializar_matriz(double **matriz, int n) {
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

void multiplicacao_sequencial(double **A, double **B, double **C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <tamanho_da_matriz>\n", argv[0]);
        return -1;
    }
    
    int n = atoi(argv[1]);
    double tempo_inicio, tempo_fim, tempo_total;
    
    double **A = alocar_matriz(n);
    double **B = alocar_matriz(n);
    double **C = alocar_matriz(n);
    
    printf("Inicializando matrizes A e B...\n");
    inicializar_matriz(A, n);
    inicializar_matriz(B, n);
    
    printf("Iniciando multiplicação sequencial...\n");
    tempo_inicio = get_time();
    
    multiplicacao_sequencial(A, B, C, n);
    
    tempo_fim = get_time();
    tempo_total = tempo_fim - tempo_inicio;
    
    printf("Tempo de execução: %.6f segundos\n", tempo_total);

    FILE *arquivo = fopen("resultado_sequencial.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d,%.6f\n", n, tempo_total);
        fclose(arquivo);
    }

    liberar_matriz(A, n);
    liberar_matriz(B, n);
    liberar_matriz(C, n);
    
    return 0;
}