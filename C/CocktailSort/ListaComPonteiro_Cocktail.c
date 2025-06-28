#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int userId;
    int movieId;
    float rating;
    long timestamp;
} Rating;

typedef struct Node {
    Rating data;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} Lista;

void inicializarLista(Lista *lista) {
    lista->head = NULL;
}

void inserirNoInicio(Lista *lista, Rating r) {
    Node *novo = (Node *)malloc(sizeof(Node));
    novo->data = r;
    novo->next = lista->head;
    lista->head = novo;
}

void liberarLista(Lista *lista) {
    Node *atual = lista->head;
    while (atual) {
        Node *tmp = atual;
        atual = atual->next;
        free(tmp);
    }
    lista->head = NULL;
}

void imprimirLista(Lista *lista) {
    printf("userId movieId rating timestamp\n");
    Node *atual = lista->head;
    while (atual) {
        printf("%5d %7d %.1f %ld\n",
               atual->data.userId,
               atual->data.movieId,
               atual->data.rating,
               atual->data.timestamp);
        atual = atual->next;
    }
}

int tamanhoLista(Lista *lista) {
    int count = 0;
    Node *atual = lista->head;
    while (atual) {
        count++;
        atual = atual->next;
    }
    return count;
}

Rating *converterParaArray(Lista *lista, int *tam) {
    *tam = tamanhoLista(lista);
    Rating *array = (Rating *)malloc((*tam) * sizeof(Rating));
    Node *atual = lista->head;
    int i = 0;
    while (atual) {
        array[i++] = atual->data;
        atual = atual->next;
    }
    return array;
}

void converterDeArray(Lista *lista, Rating *array, int tam) {
    liberarLista(lista);
    for (int i = tam - 1; i >= 0; i--) {
        inserirNoInicio(lista, array[i]);
    }
}

void cocktail_sort(Lista *lista) {
    int tam;
    Rating *array = converterParaArray(lista, &tam);
    if (tam < 2) return;

    int start = 0, end = tam - 1;
    int swapped = 1;

    while (swapped) {
        swapped = 0;

        for (int i = start; i < end; i++) {
            if (array[i].rating > array[i + 1].rating) {
                Rating tmp = array[i];
                array[i] = array[i + 1];
                array[i + 1] = tmp;
                swapped = 1;
            }
        }

        if (!swapped) break;
        swapped = 0;
        end--;

        for (int i = end; i > start; i--) {
            if (array[i - 1].rating > array[i].rating) {
                Rating tmp = array[i];
                array[i] = array[i - 1];
                array[i - 1] = tmp;
                swapped = 1;
            }
        }

        start++;
    }

    converterDeArray(lista, array, tam);
    free(array);
}

int lerCSV(Lista *lista, const char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "r");
    if (!fp) return 0;

    char linha[128];
    fgets(linha, sizeof(linha), fp); // ignora cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
        Rating r;
        sscanf(linha, "%d,%d,%f,%ld", &r.userId, &r.movieId, &r.rating, &r.timestamp);
        inserirNoInicio(lista, r);
    }

    fclose(fp);
    return 1;
}

int main() {
    Lista lista;
    inicializarLista(&lista);

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
