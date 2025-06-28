#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

void pigeonhole_sort(Lista *lista) {
    int tam;
    Rating *array = converterParaArray(lista, &tam);
    if (tam == 0) return;

    int min = (int)floor(array[0].rating);
    int max = (int)ceil(array[0].rating);

    for (int i = 1; i < tam; i++) {
        int r = (int)floor(array[i].rating);
        if (r < min) min = r;
        if (r > max) max = r;
    }

    int size = max - min + 1;
    Rating **holes = malloc(size * sizeof(Rating *));
    int *counts = calloc(size, sizeof(int));
    int *capacities = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        capacities[i] = 4;
        holes[i] = malloc(capacities[i] * sizeof(Rating));
    }

    for (int i = 0; i < tam; i++) {
        int index = (int)floor(array[i].rating) - min;
        if (counts[index] == capacities[index]) {
            capacities[index] *= 2;
            holes[index] = realloc(holes[index], capacities[index] * sizeof(Rating));
        }
        holes[index][counts[index]++] = array[i];
    }

    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < counts[i]; j++) {
            array[k++] = holes[i][j];
        }
        free(holes[i]);
    }

    free(holes);
    free(counts);
    free(capacities);

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

    printf("\nAntes da ordenação (Pigeonhole Sort):\n");
    imprimirLista(&lista);

    pigeonhole_sort(&lista);

    printf("\nDepois da ordenação (Pigeonhole Sort):\n");
    imprimirLista(&lista);

    liberarLista(&lista);
    return 0;
}
