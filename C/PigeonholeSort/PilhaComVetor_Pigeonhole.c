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

void pigeonhole_sort(PilhaVetor *pilha) {
    if (pilha->topo < 1) return;

    float min = floor(pilha->dados[0].rating);
    float max = ceil(pilha->dados[0].rating);

    for (int i = 1; i <= pilha->topo; i++) {
        float r = floor(pilha->dados[i].rating);
        if (r < min) min = r;
        if (r > max) max = r;
    }

    int size = (int)(max - min + 1);
    Rating **holes = (Rating **)malloc(size * sizeof(Rating *));
    int *counts = (int *)calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        holes[i] = (Rating *)malloc((pilha->topo + 1) * sizeof(Rating));
    }

    for (int i = 0; i <= pilha->topo; i++) {
        int idx = (int)(floor(pilha->dados[i].rating) - min);
        holes[idx][counts[idx]++] = pilha->dados[i];
    }

    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < counts[i]; j++) {
            pilha->dados[k++] = holes[i][j];
        }
        free(holes[i]);
    }

    pilha->topo = k - 1;

    free(holes);
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
    inicializarPilha(&pilha, 1000); // capacidade inicial

    if (!lerCSV(&pilha, "ratings.csv")) {
        printf("Erro ao abrir ratings.csv\n");
        return 1;
    }

    printf("\nAntes do Pigeonhole Sort:\n");
    imprimirPilha(&pilha);

    pigeonhole_sort(&pilha);

    printf("\nDepois do Pigeonhole Sort:\n");
    imprimirPilha(&pilha);

    liberarPilha(&pilha);
    return 0;
}
