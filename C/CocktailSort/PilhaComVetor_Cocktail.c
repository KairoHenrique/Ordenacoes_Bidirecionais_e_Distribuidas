
#include <stdio.h>
#include <stdlib.h>

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
        return vazio; // erro: pilha vazia
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

void cocktail_sort(PilhaVetor *pilha) {
    if (pilha->topo < 1) return;

    int start_idx = 0;
    int end_idx = pilha->topo;
    int swapped = 1;

    while (swapped) {
        swapped = 0;
        for (int i = start_idx; i < end_idx; i++) {
            if (pilha->dados[i].rating > pilha->dados[i + 1].rating) {
                Rating tmp = pilha->dados[i];
                pilha->dados[i] = pilha->dados[i + 1];
                pilha->dados[i + 1] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
        swapped = 0;
        end_idx--;

        for (int i = end_idx - 1; i >= start_idx; i--) {
            if (pilha->dados[i].rating > pilha->dados[i + 1].rating) {
                Rating tmp = pilha->dados[i];
                pilha->dados[i] = pilha->dados[i + 1];
                pilha->dados[i + 1] = tmp;
                swapped = 1;
            }
        }
        start_idx++;
    }
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

    printf("\nAntes do Cocktail Sort:\n");
    imprimirPilha(&pilha);

    cocktail_sort(&pilha);

    printf("\nDepois do Cocktail Sort:\n");
    imprimirPilha(&pilha);

    liberarPilha(&pilha);
    return 0;
}