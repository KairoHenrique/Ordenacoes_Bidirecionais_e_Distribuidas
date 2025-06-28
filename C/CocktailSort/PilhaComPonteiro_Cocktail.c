#include <stdio.h>
#include <stdlib.h>

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

void cocktail_sort(Pilha *pilha) {
    int n;
    Rating *arr = converterParaArray(pilha, &n);
    if (n <= 1) return;

    int start = 0, end = n - 1;
    int swapped = 1;

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

        for (int i = end; i > start; i--) {
            if (arr[i - 1].rating > arr[i].rating) {
                Rating temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                swapped = 1;
            }
        }

        start++;
    }

    converterDeArray(pilha, arr, n);
    free(arr);
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

    printf("\nAntes do Cocktail Sort:\n");
    imprimirPilha(&pilha);

    cocktail_sort(&pilha);

    printf("\nDepois do Cocktail Sort:\n");
    imprimirPilha(&pilha);

    liberarPilha(&pilha);
    return 0;
}
