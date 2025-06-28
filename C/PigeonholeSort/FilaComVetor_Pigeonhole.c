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
               fila->dados[i].timestamp);
    }
}

// Pigeonhole Sort baseado no campo rating
void pigeonhole_sort(FilaVetor *fila) {
    if (fila->tamanho == 0) return;

    int min = (int)floor(fila->dados[0].rating);
    int max = (int)ceil(fila->dados[0].rating);

    for (int i = 1; i < fila->tamanho; i++) {
        int val = (int)floor(fila->dados[i].rating);
        if (val < min) min = val;
        if (val > max) max = val;
    }

    int size = max - min + 1;
    Rating** holes = malloc(size * sizeof(Rating*));
    int* counts = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        holes[i] = malloc(fila->tamanho * sizeof(Rating));
    }

    for (int i = 0; i < fila->tamanho; i++) {
        int index = (int)floor(fila->dados[i].rating) - min;
        holes[index][counts[index]++] = fila->dados[i];
    }

    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < counts[i]; j++) {
            fila->dados[k++] = holes[i][j];
        }
    }

    fila->tamanho = k;

    for (int i = 0; i < size; i++) {
        free(holes[i]);
    }
    free(holes);
    free(counts);
}

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

    printf("\nAntes do Pigeonhole Sort:\n");
    imprimirFila(&fila);

    pigeonhole_sort(&fila);

    printf("\nDepois do Pigeonhole Sort:\n");
    imprimirFila(&fila);

    return 0;
}
