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

void pigeonhole_sort(Fila *f) {
    int n;
    Rating *arr = to_array(f, &n);
    if (n == 0) return;

    int min = (int)floor(arr[0].rating);
    int max = (int)ceil(arr[0].rating);

    for (int i = 1; i < n; i++) {
        int val = (int)floor(arr[i].rating);
        if (val < min) min = val;
        if (val > max) max = val;
    }

    int size = max - min + 1;
    Rating **holes = malloc(size * sizeof(Rating *));
    int *counts = calloc(size, sizeof(int));
    int *capacities = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        capacities[i] = 4;
        holes[i] = malloc(capacities[i] * sizeof(Rating));
    }

    for (int i = 0; i < n; i++) {
        int index = (int)floor(arr[i].rating) - min;
        if (counts[index] == capacities[index]) {
            capacities[index] *= 2;
            holes[index] = realloc(holes[index], capacities[index] * sizeof(Rating));
        }
        holes[index][counts[index]++] = arr[i];
    }

    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < counts[i]; j++) {
            arr[k++] = holes[i][j];
        }
        free(holes[i]);
    }

    free(holes);
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

    pigeonhole_sort(&fila);

    printf("\nDepois da ordenação (Pigeonhole Sort por rating):\n");
    imprimirFila(&fila);

    liberarFila(&fila);
    return 0;
}
