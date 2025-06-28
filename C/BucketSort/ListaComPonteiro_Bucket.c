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

// Função auxiliar para ordenação
int compararPorRating(const void *a, const void *b) {
    float ra = ((Rating *)a)->rating;
    float rb = ((Rating *)b)->rating;
    return (ra > rb) - (ra < rb);
}

void bucket_sort(Lista *lista, float bucket_size) {
    int tam;
    Rating *array = converterParaArray(lista, &tam);
    if (tam == 0) return;

    float min = array[0].rating;
    float max = array[0].rating;

    for (int i = 1; i < tam; i++) {
        if (array[i].rating < min) min = array[i].rating;
        if (array[i].rating > max) max = array[i].rating;
    }

    int bucket_count = (int)((max - min) / bucket_size) + 1;

    // Criação dos buckets
    Rating **buckets = (Rating **)malloc(bucket_count * sizeof(Rating *));
    int *counts = (int *)calloc(bucket_count, sizeof(int));

    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = (Rating *)malloc(tam * sizeof(Rating));
    }

    // Distribuição
    for (int i = 0; i < tam; i++) {
        int idx = (int)((array[i].rating - min) / bucket_size);
        buckets[idx][counts[idx]++] = array[i];
    }

    // Ordena e concatena
    int k = 0;
    for (int i = 0; i < bucket_count; i++) {
        qsort(buckets[i], counts[i], sizeof(Rating), compararPorRating);
        for (int j = 0; j < counts[i]; j++) {
            array[k++] = buckets[i][j];
        }
    }

    converterDeArray(lista, array, k);

    // Limpeza
    for (int i = 0; i < bucket_count; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(counts);
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

    printf("\nAntes do Bucket Sort:\n");
    imprimirLista(&lista);

    bucket_sort(&lista, 1.0); // bucket size = 1.0

    printf("\nDepois do Bucket Sort:\n");
    imprimirLista(&lista);

    liberarLista(&lista);
    return 0;
}
