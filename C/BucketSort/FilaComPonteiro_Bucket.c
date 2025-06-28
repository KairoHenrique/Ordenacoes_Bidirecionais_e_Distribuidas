#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Rating {
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
    Node *head, *tail;
} Fila;

void inicializarFila(Fila *f) {
    f->head = f->tail = NULL;
}

void enqueue(Fila *f, Rating r) {
    Node *novo = (Node *)malloc(sizeof(Node));
    novo->data = r;
    novo->next = NULL;

    if (f->tail) {
        f->tail->next = novo;
    } else {
        f->head = novo;
    }
    f->tail = novo;
}

Rating* to_array(Fila *f, int *n) {
    int count = 0;
    Node *temp = f->head;
    while (temp) {
        count++;
        temp = temp->next;
    }

    Rating *arr = malloc(count * sizeof(Rating));
    temp = f->head;
    for (int i = 0; i < count; i++) {
        arr[i] = temp->data;
        temp = temp->next;
    }
    *n = count;
    return arr;
}

void from_array(Fila *f, Rating *arr, int n) {
    f->head = f->tail = NULL;
    for (int i = 0; i < n; i++) {
        enqueue(f, arr[i]);
    }
}

// Função de comparação para qsort
int compararRatings(const void *a, const void *b) {
    float r1 = ((Rating *)a)->rating;
    float r2 = ((Rating *)b)->rating;
    return (r1 > r2) - (r1 < r2);  // retorna 1, -1 ou 0
}

void bucket_sort(Fila *f, float bucket_size) {
    int n;
    Rating *arr = to_array(f, &n);
    if (n == 0) return;

    float min = arr[0].rating, max = arr[0].rating;
    for (int i = 1; i < n; i++) {
        if (arr[i].rating < min) min = arr[i].rating;
        if (arr[i].rating > max) max = arr[i].rating;
    }

    int bucket_count = (int)((max - min) / bucket_size) + 1;
    Rating **buckets = malloc(bucket_count * sizeof(Rating *));
    int *counts = calloc(bucket_count, sizeof(int));
    int *capacities = calloc(bucket_count, sizeof(int));

    for (int i = 0; i < bucket_count; i++) {
        capacities[i] = 4;
        buckets[i] = malloc(capacities[i] * sizeof(Rating));
    }

    for (int i = 0; i < n; i++) {
        int idx = (int)((arr[i].rating - min) / bucket_size);
        if (counts[idx] == capacities[idx]) {
            capacities[idx] *= 2;
            buckets[idx] = realloc(buckets[idx], capacities[idx] * sizeof(Rating));
        }
        buckets[idx][counts[idx]++] = arr[i];
    }

    int k = 0;
    for (int i = 0; i < bucket_count; i++) {
        qsort(buckets[i], counts[i], sizeof(Rating), compararRatings);
        for (int j = 0; j < counts[i]; j++) {
            arr[k++] = buckets[i][j];
        }
        free(buckets[i]);
    }

    free(buckets);
    free(counts);
    free(capacities);

    from_array(f, arr, n);
    free(arr);
}

void imprimirFila(Fila *f) {
    Node *atual = f->head;
    printf("userId movieId rating timestamp\n");
    while (atual) {
        printf("%5d %7d %.1f %ld\n", atual->data.userId, atual->data.movieId, atual->data.rating, atual->data.timestamp);
        atual = atual->next;
    }
}

int lerCSV(Fila *f, const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (!fp) return 0;

    char linha[128];
    fgets(linha, sizeof(linha), fp); // Ignora cabeçalho

    while (fgets(linha, sizeof(linha), fp)) {
        Rating r;
        sscanf(linha, "%d,%d,%f,%ld", &r.userId, &r.movieId, &r.rating, &r.timestamp);
        enqueue(f, r);
    }

    fclose(fp);
    return 1;
}

void liberarFila(Fila *f) {
    Node *temp;
    while (f->head) {
        temp = f->head;
        f->head = f->head->next;
        free(temp);
    }
    f->tail = NULL;
}

int main() {
    Fila fila;
    inicializarFila(&fila);

    if (!lerCSV(&fila, "ratings.csv")) {
        printf("Erro ao abrir ratings.csv\n");
        return 1;
    }

    printf("\nAntes da ordenação:\n");
    imprimirFila(&fila);

    bucket_sort(&fila, 1.0f);  // Tamanho do bucket pode ser ajustado

    printf("\nDepois da ordenação (Bucket Sort por rating):\n");
    imprimirFila(&fila);

    liberarFila(&fila);
    return 0;
}
