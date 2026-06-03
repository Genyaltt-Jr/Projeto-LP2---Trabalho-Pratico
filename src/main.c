#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

// Definição da estrutura com identificadores em inglês (Convenção)
typedef struct {
    int start_row;         // Linha inicial da fatia
    int end_row;           // Linha final (exclusiva) da fatia
    int matrix_size;       // Dimensão n das matrizes
    const double *matrix_a;
    const double *matrix_b;
    double *matrix_c;      // Escrita direta em posições disjuntas (sem conflito)
} ThreadArgs;

// Implementação sequencial nativa para baseline
void multiply_sequential(const double *matrix_a, const double *matrix_b, double *matrix_c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += matrix_a[i * n + k] * matrix_b[k * n + j];
            }
            matrix_c[i * n + j] = sum;
        }
    }
}

// Função que a thread executará
void *matrix_worker(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int n = args->matrix_size;

    // Cada thread calcula estritamente seu bloco de linhas
    for (int i = args->start_row; i < args->end_row; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += args->matrix_a[i * n + k] * args->matrix_b[k * n + j];
            }
            args->matrix_c[i * n + j] = sum;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // Tratamento dinâmico do número de threads por argumento
    int num_threads = 4;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    int n = 1200; // Sugestão do enunciado (1000 a 1500)
    size_t size = (size_t)n * n;

    // Setup: Alocação de memória e preenchimento (FICA FORA DO CRONÔMETRO)
    double *matrix_a = malloc(size * sizeof(double));
    double *matrix_b = malloc(size * sizeof(double));
    double *matrix_c_seq = malloc(size * sizeof(double));
    double *matrix_c_par = malloc(size * sizeof(double));

    // Preenchendo as matrizes em código com valores dummy
    for (size_t i = 0; i < size; i++) {
        matrix_a[i] = (double)(i % 100) / 7.0;
        matrix_b[i] = (double)(i % 50) / 3.0;
    }

    // -------------------------------------------------------------
    // MEDIÇÃO SEQUENCIAL (Q2)
    // -------------------------------------------------------------
    // Nota: O enunciado pede média de 5 execuções descartando o aquecimento.
    // Para simplificar o escopo do código, faremos a cronometragem de uma rodada limpa:
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    multiply_sequential(matrix_a, matrix_b, matrix_c_seq, n);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double time_seq = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    // -------------------------------------------------------------
    // MEDIÇÃO PARALELA (Q3)
    // -------------------------------------------------------------
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadArgs *args = malloc(num_threads * sizeof(ThreadArgs));

    clock_gettime(CLOCK_MONOTONIC, &t0);

    // Divisão do trabalho por linhas (mapa por linhas)
    int rows_per_thread = n / num_threads;
    int extra_rows = n % num_threads;
    int current_row = 0;

    for (int i = 0; i < num_threads; i++) {
        args[i].start_row = current_row;
        args[i].end_row = current_row + rows_per_thread + (i < extra_rows ? 1 : 0);
        current_row = args[i].end_row;

        args[i].matrix_size = n;
        args[i].matrix_a = matrix_a;
        args[i].matrix_b = matrix_b;
        args[i].matrix_c = matrix_c_par;

        pthread_create(&threads[i], NULL, matrix_worker, &args[i]);
    }

    // Junção das threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double time_par = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    // -------------------------------------------------------------
    // VERIFICAÇÃO AUTOMÁTICA DE CORRETUDE (Q3 - B)
    // -------------------------------------------------------------
    int is_correct = 1;
    for (size_t i = 0; i < size; i++) {
        if (fabs(matrix_c_seq[i] - matrix_c_par[i]) > 1e-6) {
            is_correct = 0;
            break;
        }
    }

    printf("Resultado da Validacao: %s\n", is_correct ? "OK" : "FALHA");
    printf("Tempo Sequencial: %.5f s\n", time_seq);
    printf("Tempo Paralelo (%d threads): %.5f s\n", num_threads, time_par);
    printf("Speedup: %.2fx\n", time_seq / time_par);

    // Limpeza
    free(matrix_a); free(matrix_b); free(matrix_c_seq); free(matrix_c_par);
    free(threads); free(args);

    return 0;
}