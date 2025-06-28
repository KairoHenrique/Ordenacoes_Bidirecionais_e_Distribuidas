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

void cocktail_sort(ListaVetor *lista) {
    if (lista->tamanho < 2) return;

    int start = 0;
    int end = lista->tamanho - 1;
    int swapped = 1;

    while (swapped) {
        swapped = 0;

        for (int i = start; i < end; i++) {
            if (lista->dados[i].rating > lista->dados[i + 1].rating) {
                Rating temp = lista->dados[i];
                lista->dados[i] = lista->dados[i + 1];
                lista->dados[i + 1] = temp;
                swapped = 1;
            }
        }

        if (!swapped) break;

        swapped = 0;
        end--;

        for (int i = end; i > start; i--) {
            if (lista->dados[i - 1].rating > lista->dados[i].rating) {
                Rating temp = lista->dados[i];
                lista->dados[i] = lista->dados[i - 1];
                lista->dados[i - 1] = temp;
                swapped = 1;
            }
        }
        start++;
    }
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

    printf("\nAntes do Cocktail Sort:\n");
    imprimirLista(&lista);

    cocktail_sort(&lista);

    printf("\nDepois do Cocktail Sort:\n");
    imprimirLista(&lista);

    liberarLista(&lista);
    return 0;
}
