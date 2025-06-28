#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_RATINGS 10000

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

// Função de comparação para qsort
int compararPorRating(const void *a, const void *b) {
    float ra = ((Rating *)a)->rating;
    float rb = ((Rating *)b)->rating;
    return (ra > rb) - (ra < rb);
}

void bucket_sort(ListaVetor *lista, float bucket_size) {
    if (lista->tamanho == 0) return;

    float min = lista->dados[0].rating;
    float max = lista->dados[0].rating;

    for (int i = 1; i < lista->tamanho; i++) {
        if (lista->dados[i].rating < min) min = lista->dados[i].rating;
        if (lista->dados[i].rating > max) max = lista->dados[i].rating;
    }

    int bucket_count = (int)((max - min) / bucket_size) + 1;

    Rating **buckets = (Rating **)malloc(bucket_count * sizeof(Rating *));
    int *counts = (int *)calloc(bucket_count, sizeof(int));

    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = (Rating *)malloc(lista->tamanho * sizeof(Rating));
    }

    // Distribui
    for (int i = 0; i < lista->tamanho; i++) {
        int idx = (int)((lista->dados[i].rating - min) / bucket_size);
        buckets[idx][counts[idx]++] = lista->dados[i];
    }

    // Ordena e reconstrói a lista
    int k = 0;
    for (int i = 0; i < bucket_count; i++) {
        qsort(buckets[i], counts[i], sizeof(Rating), compararPorRating);
        for (int j = 0; j < counts[i]; j++) {
            lista->dados[k++] = buckets[i][j];
        }
        free(buckets[i]);
    }

    free(buckets);
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
    inicializarLista(&lista, 1000);

    if (!lerCSV(&lista, "ratings.csv")) {
        printf("Erro ao abrir ratings.csv\n");
        return 1;
    }

    printf("\nAntes do Bucket Sort:\n");
    imprimirLista(&lista);

    bucket_sort(&lista, 1.0); // bucket_size de 1.0

    printf("\nDepois do Bucket Sort:\n");
    imprimirLista(&lista);

    liberarLista(&lista);
    return 0;
}
