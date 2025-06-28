#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->head = f->tail = NULL;
}

// Enfileira um Rating
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

// Converte a fila para array
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

// Reconstrói a fila a partir de um array
void from_array(Fila *f, Rating *arr, int n) {
    f->head = f->tail = NULL;
    for (int i = 0; i < n; i++) {
        enqueue(f, arr[i]);
    }
}

// Cocktail Sort para Rating por campo `rating`
void cocktail_sort(Fila *f) {
    int n;
    Rating *arr = to_array(f, &n);

    int swapped = 1;
    int start = 0;
    int end = n - 1;

    while (swapped) {
        swapped = 0;

        for (int i = start; i < end; i++) {
            if (arr[i].rating > arr[i + 1].rating) {
                Rating temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;
            }
        }

        if (!swapped) break;

        swapped = 0;
        end--;

        for (int i = end - 1; i >= start; i--) {
            if (arr[i].rating > arr[i + 1].rating) {
                Rating temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                swapped = 1;
            }
        }
        start++;
    }

    from_array(f, arr, n);
    free(arr);
}

// Imprime os ratings
void imprimirFila(Fila *f) {
    Node *atual = f->head;
    printf("userId movieId rating timestamp\n");
    while (atual) {
        printf("%5d %7d %.1f %ld\n", atual->data.userId, atual->data.movieId, atual->data.rating, atual->data.timestamp);
        atual = atual->next;
    }
}

// Lê o CSV
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

    cocktail_sort(&fila);

    printf("\nDepois da ordenação (Cocktail Sort por rating):\n");
    imprimirFila(&fila);

    liberarFila(&fila);
    return 0;
}
