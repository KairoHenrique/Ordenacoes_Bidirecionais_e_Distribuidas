#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_RATINGS 10000  // ou um valor maior dependendo do tamanho do CSV

typedef struct {
    int userId;
    int movieId;
    float rating;
    long timestamp;
} Rating;

typedef struct {
    Rating *dados;
    int tamanho;
    int capacidade;
} ListaVetor;

void inicializarLista(ListaVetor *lista, int capacidadeInicial) {
    lista->dados = (Rating *)malloc(capacidadeInicial * sizeof(Rating));
    lista->tamanho = 0;
    lista->capacidade = capacidadeInicial;
}

void adicionar(ListaVetor *lista, Rating r) {
    if (lista->tamanho >= lista->capacidade) {
        lista->capacidade *= 2;
        lista->dados = realloc(lista->dados, lista->capacidade * sizeof(Rating));
    }
    lista->dados[lista->tamanho++] = r;
}

void liberarLista(ListaVetor *lista) {
    free(lista->dados);
    lista->tamanho = 0;
    lista->capacidade = 0;
}

void imprimirLista(ListaVetor *lista) {
    printf("userId movieId rating timestamp\n");
    for (int i = 0; i < lista->tamanho; i++) {
        printf("%5d %7d %.1f %ld\n",
               lista->dados[i].userId,
               lista->dados[i].movieId,
               lista->dados[i].rating,
               lista->dados[i].timestamp);
    }
}

void pigeonhole_sort(ListaVetor *lista) {
    if (lista->tamanho == 0) return;

    int min = floor(lista->dados[0].rating);
    int max = ceil(lista->dados[0].rating);

    for (int i = 1; i < lista->tamanho; i++) {
        int r = floor(lista->dados[i].rating);
        if (r < min) min = r;
        if (r > max) max = r;
    }

    int size = max - min + 1;

    Rating **holes = (Rating **)malloc(size * sizeof(Rating *));
    int *counts = (int *)calloc(size, sizeof(int));
    for (int i = 0; i < size; i++) {
        holes[i] = (Rating *)malloc(lista->tamanho * sizeof(Rating));
    }

    // Distribui
    for (int i = 0; i < lista->tamanho; i++) {
        int index = (int)floor(lista->dados[i].rating) - min;
        holes[index][counts[index]++] = lista->dados[i];
    }

    // Reconstrói lista
    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < counts[i]; j++) {
            lista->dados[k++] = holes[i][j];
        }
        free(holes[i]);
    }

    free(holes);
    free(counts);
}

int lerCSV(ListaVetor *lista, const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return 0;

    char linha[128];
    fgets(linha, sizeof(linha), fp); // ignora cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
        Rating r;
        sscanf(linha, "%d,%d,%f,%ld", &r.userId, &r.movieId, &r.rating, &r.timestamp);
        adicionar(lista, r);
    }

    fclose(fp);
    return 1;
}

int main() {
    ListaVetor lista;
    inicializarLista(&lista, 1000); // capacidade inicial

    if (!lerCSV(&lista, "ratings.csv")) {
        printf("Erro ao abrir ratings.csv\n");
        return 1;
    }

    printf("\nAntes do Pigeonhole Sort:\n");
    imprimirLista(&lista);

    pigeonhole_sort(&lista);

    printf("\nDepois do Pigeonhole Sort:\n");
    imprimirLista(&lista);

    liberarLista(&lista);
    return 0;
}
