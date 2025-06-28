#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 10000 // capacidade máxima da fila

typedef struct {
    int userId;
    int movieId;
    float rating;
    long timestamp;
} Rating;

typedef struct {
    Rating dados[MAX];
    int tamanho;
} FilaVetor;

void inicializarFila(FilaVetor *fila) {
    fila->tamanho = 0;
}

void enqueue(FilaVetor *fila, Rating r) {
    if (fila->tamanho < MAX) {
        fila->dados[fila->tamanho++] = r;
    }
}

Rating dequeue(FilaVetor *fila) {
    Rating r = {-1, -1, -1.0, -1};
    if (fila->tamanho == 0) return r;

    r = fila->dados[0];
    for (int i = 1; i < fila->tamanho; i++) {
        fila->dados[i - 1] = fila->dados[i];
    }
    fila->tamanho--;
    return r;
}

void imprimirFila(FilaVetor *fila) {
    printf("userId movieId rating timestamp\n");
    for (int i = 0; i < fila->tamanho; i++) {
        printf("%5d %7d %.1f %ld\n",
            fila->dados[i].userId,
            fila->dados[i].movieId,
            fila->dados[i].rating,
            fila->dados[i].timestamp
        );
    }
}

// Função auxiliar para comparação de Ratings
int compararPorRating(const void *a, const void *b) {
    float ra = ((Rating *)a)->rating;
    float rb = ((Rating *)b)->rating;
    return (ra > rb) - (ra < rb);
}

// Bucket Sort com vetor
void bucket_sort(FilaVetor *fila, float bucket_size) {
    if (fila->tamanho == 0) return;

    float min = fila->dados[0].rating;
    float max = fila->dados[0].rating;

    for (int i = 1; i < fila->tamanho; i++) {
        if (fila->dados[i].rating < min) min = fila->dados[i].rating;
        if (fila->dados[i].rating > max) max = fila->dados[i].rating;
    }

    int bucket_count = (int)((max - min) / bucket_size) + 1;

    // Aloca os buckets
    Rating **buckets = (Rating **)malloc(bucket_count * sizeof(Rating *));
    int *counts = (int *)calloc(bucket_count, sizeof(int));

    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = (Rating *)malloc(fila->tamanho * sizeof(Rating));
    }

    // Distribui os dados nos buckets
    for (int i = 0; i < fila->tamanho; i++) {
        int idx = (int)((fila->dados[i].rating - min) / bucket_size);
        buckets[idx][counts[idx]++] = fila->dados[i];
    }

    // Ordena os buckets individualmente e concatena
    int k = 0;
    for (int i = 0; i < bucket_count; i++) {
        qsort(buckets[i], counts[i], sizeof(Rating), compararPorRating);
        for (int j = 0; j < counts[i]; j++) {
            fila->dados[k++] = buckets[i][j];
        }
    }

    fila->tamanho = k;

    // Liberação de memória
    for (int i = 0; i < bucket_count; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(counts);
}

// Lê os dados do CSV
int lerCSV(FilaVetor *fila, const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return 0;

    char linha[128];
    fgets(linha, sizeof(linha), fp); // pula cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
        Rating r;
        sscanf(linha, "%d,%d,%f,%ld", &r.userId, &r.movieId, &r.rating, &r.timestamp);
        enqueue(fila, r);
    }

    fclose(fp);
    return 1;
}

int main() {
    FilaVetor fila;
    inicializarFila(&fila);

    if (!lerCSV(&fila, "ratings.csv")) {
        printf("Erro ao abrir ratings.csv\n");
        return 1;
    }

    printf("\nAntes do Bucket Sort:\n");
    imprimirFila(&fila);

    bucket_sort(&fila, 1.0);  // bucket_size de 1.0

    printf("\nDepois do Bucket Sort:\n");
    imprimirFila(&fila);

    return 0;
}
