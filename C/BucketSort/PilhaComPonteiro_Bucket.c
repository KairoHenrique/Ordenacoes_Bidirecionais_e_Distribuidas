#include <stdio.h>
#include <stdlib.h>
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
    Node *top;
} Pilha;

void inicializarPilha(Pilha *pilha) {
    pilha->top = NULL;
}

void push(Pilha *pilha, Rating r) {
    Node *novo = (Node *)malloc(sizeof(Node));
    novo->data = r;
    novo->next = pilha->top;
    pilha->top = novo;
}

void liberarPilha(Pilha *pilha) {
    Node *atual = pilha->top;
    while (atual) {
        Node *tmp = atual;
        atual = atual->next;
        free(tmp);
    }
    pilha->top = NULL;
}

int tamanhoPilha(Pilha *pilha) {
    int count = 0;
    Node *atual = pilha->top;
    while (atual) {
        count++;
        atual = atual->next;
    }
    return count;
}

Rating *converterParaArray(Pilha *pilha, int *tam) {
    *tam = tamanhoPilha(pilha);
    Rating *array = (Rating *)malloc((*tam) * sizeof(Rating));
    Node *atual = pilha->top;
    int i = 0;
    while (atual) {
        array[i++] = atual->data;
        atual = atual->next;
    }
    return array;
}

void converterDeArray(Pilha *pilha, Rating *array, int tam) {
    liberarPilha(pilha);
    for (int i = tam - 1; i >= 0; i--) {
        push(pilha, array[i]);
    }
}

void imprimirPilha(Pilha *pilha) {
    printf("userId movieId rating timestamp\n");
    Node *atual = pilha->top;
    while (atual) {
        printf("%5d %7d %.1f %ld\n",
               atual->data.userId,
               atual->data.movieId,
               atual->data.rating,
               atual->data.timestamp);
        atual = atual->next;
    }
}

int compararPorRating(const void *a, const void *b) {
    float ra = ((Rating *)a)->rating;
    float rb = ((Rating *)b)->rating;
    return (ra > rb) - (ra < rb);
}

void bucket_sort(Pilha *pilha, float bucket_size) {
    int tam;
    Rating *array = converterParaArray(pilha, &tam);
    if (tam == 0) return;

    float min = array[0].rating;
    float max = array[0].rating;

    for (int i = 1; i < tam; i++) {
        if (array[i].rating < min) min = array[i].rating;
        if (array[i].rating > max) max = array[i].rating;
    }

    int bucket_count = (int)((max - min) / bucket_size) + 1;

    Rating **buckets = (Rating **)malloc(bucket_count * sizeof(Rating *));
    int *counts = (int *)calloc(bucket_count, sizeof(int));
    for (int i = 0; i < bucket_count; i++) {
        buckets[i] = (Rating *)malloc(tam * sizeof(Rating));
    }

    for (int i = 0; i < tam; i++) {
        int idx = (int)((array[i].rating - min) / bucket_size);
        buckets[idx][counts[idx]++] = array[i];
    }

    int k = 0;
    for (int i = 0; i < bucket_count; i++) {
        qsort(buckets[i], counts[i], sizeof(Rating), compararPorRating);
        for (int j = 0; j < counts[i]; j++) {
            array[k++] = buckets[i][j];
        }
        free(buckets[i]);
    }

    free(buckets);
    free(counts);

    converterDeArray(pilha, array, k);
    free(array);
}

int lerCSV(Pilha *pilha, const char *nomeArquivo) {
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
    Pilha pilha;
    inicializarPilha(&pilha);

    if (!lerCSV(&pilha, "ratings.csv")) {
        printf("Erro ao abrir ratings.csv\n");
        return 1;
    }

    printf("\nAntes do Bucket Sort:\n");
    imprimirPilha(&pilha);

    bucket_sort(&pilha, 1.0);  // tamanho do bucket

    printf("\nDepois do Bucket Sort:\n");
    imprimirPilha(&pilha);

    liberarPilha(&pilha);
    return 0;
}
