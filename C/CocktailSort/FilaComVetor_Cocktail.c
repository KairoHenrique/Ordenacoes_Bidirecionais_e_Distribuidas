#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Cocktail Sort baseado no campo rating
void cocktail_sort(FilaVetor *fila) {
    if (fila->tamanho < 2) return;

    int start = 0, end = fila->tamanho - 1;
    int swapped = 1;

    while (swapped) {
        swapped = 0;

        for (int i = start; i < end; i++) {
            if (fila->dados[i].rating > fila->dados[i + 1].rating) {
                Rating temp = fila->dados[i];
                fila->dados[i] = fila->dados[i + 1];
                fila->dados[i + 1] = temp;
                swapped = 1;
            }
        }

        if (!swapped) break;

        swapped = 0;
        end--;

        for (int i = end; i > start; i--) {
            if (fila->dados[i - 1].rating > fila->dados[i].rating) {
                Rating temp = fila->dados[i];
                fila->dados[i] = fila->dados[i - 1];
                fila->dados[i - 1] = temp;
                swapped = 1;
            }
        }

        start++;
    }
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

    printf("\nAntes do Cocktail Sort:\n");
    imprimirFila(&fila);

    cocktail_sort(&fila);

    printf("\nDepois do Cocktail Sort:\n");
    imprimirFila(&fila);

    return 0;
}
