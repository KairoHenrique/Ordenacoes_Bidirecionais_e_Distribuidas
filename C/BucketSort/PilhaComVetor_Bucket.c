#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int userId;
    int movieId;
    float rating;
    long timestamp;
} Rating;

typedef struct {
    Rating *dados;
    int topo;
    int capacidade;
} PilhaVetor;

void inicializarPilha(PilhaVetor *pilha, int capacidadeInicial) {
    pilha->dados = (Rating *)malloc(capacidadeInicial * sizeof(Rating));
    pilha->topo = -1;
    pilha->capacidade = capacidadeInicial;
}

void push(PilhaVetor *pilha, Rating r) {
    if (pilha->topo + 1 >= pilha->capacidade) {
        pilha->capacidade *= 2;
        pilha->dados = realloc(pilha->dados, pilha->capacidade * sizeof(Rating));
    }
    pilha->dados[++(pilha->topo)] = r;
}

Rating pop(PilhaVetor *pilha) {
    if (pilha->topo < 0) {
        Rating vazio = {0, 0, 0.0, 0};
        return vazio;
    }
    return pilha->dados[(pilha->topo)--];
}

void liberarPilha(PilhaVetor *pilha) {
    free(pilha->dados);
    pilha->topo = -1;
    pilha->capacidade = 0;
}

void imprimirPilha(PilhaVetor *pilha) {
    printf("userId movieId rating timestamp\n");
    for (int i = pilha->topo; i >= 0; i--) {
        printf("%5d %7d %.1f %ld\n",
               pilha->dados[i].userId,
               pilha->dados[i].movieId,
               pilha->dados[i].rating,
               pilha->dados[i].timestamp);
    }
}

// Comparador usado pelo qsort
int compararRating(const void *a, const void *b) {
    float ra = ((Rating *)a)->rating;
    float rb = ((Rating *)b)->rating;
    return (ra > rb) - (ra < rb);
}

void bucket_sort(PilhaVetor *pilha, float bucket_size) {
    if (pilha->topo < 1) return;

    float min = pilha->dados[0].rating;
    float max = pilha->dados[0].rating;

    for (int i = 1; i <= pilha->topo; i++) {
        if (pilha->dados[i].rating < min) min = pilha->dados[i].rating;
        if (pilha->dados[i].rating > max) max = pilha->dados[i].rating;
    }

    int bucket_count = (int)((max - min) / bucket_size) + 1;

    // Aloca os buckets
    Rating **buckets = (Rating **)malloc(bucket_count * sizeof(Rating *));
    int *counts = (int *)calloc(bucket_count, sizeof(int));
    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = (Rating *)malloc((pilha->topo + 1) * sizeof(Rating));
    }

    // Distribuição
    for (int i = 0; i <= pilha->topo; i++) {
        int idx = (int)((pilha->dados[i].rating - min) / bucket_size);
        buckets[idx][counts[idx]++] = pilha->dados[i];
    }

    // Ordena e reconstrói a pilha
    int k = 0;
    for (int i = 0; i < bucket_count; i++) {
        qsort(buckets[i], counts[i], sizeof(Rating), compararRating);
        for (int j = 0; j < counts[i]; j++) {
            pilha->dados[k++] = buckets[i][j];
        }
        free(buckets[i]);
    }

    pilha->topo = k - 1;

    free(buckets);
    free(counts);
}

int lerCSV(PilhaVetor *pilha, const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return 0;

    char linha[128];
    fgets(linha, sizeof(linha), fp); // ignora cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
        Rating r;
        sscanf(linha, "%d,%d,%f,%ld", &r.userId, &r.movieId, &r.rating, &r.timestamp);
        push(pilha, r);
    }

    fclose(fp);
    return 1;
}

int main() {
    PilhaVetor pilha;
    inicializarPilha(&pilha, 1000);

    if (!lerCSV(&pilha, "ratings.csv")) {
        printf("Erro ao abrir ratings.csv\n");
        return 1;
    }

    printf("\nAntes do Bucket Sort:\n");
    imprimirPilha(&pilha);

    bucket_sort(&pilha, 1.0); // bucket_size = 1.0

    printf("\nDepois do Bucket Sort:\n");
    imprimirPilha(&pilha);

    liberarPilha(&pilha);
    return 0;
}
