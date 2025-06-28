#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Rating {
    int userId;
    int movieId;
    float rating;
    long timestamp;
};

struct Node {
    Rating data;
    Node* next;
    Node(Rating r) : data(r), next(nullptr) {}
};

class Fila {
private:
    Node *head, *tail;

public:
    Fila() : head(nullptr), tail(nullptr) {}

    void enqueue(Rating r) {
        Node* novo = new Node(r);
        if (tail) {
            tail->next = novo;
        } else {
            head = novo;
        }
        tail = novo;
    }

    Rating* toArray(int& n) {
        n = 0;
        Node* temp = head;
        while (temp) {
            n++;
            temp = temp->next;
        }

        Rating* arr = new Rating[n];
        temp = head;
        for (int i = 0; i < n; i++) {
            arr[i] = temp->data;
            temp = temp->next;
        }
        return arr;
    }

    void fromArray(Rating* arr, int n) {
        liberar();
        for (int i = 0; i < n; i++) {
            enqueue(arr[i]);
        }
    }

    void cocktail_sort() {
        int n;
        Rating* arr = toArray(n);

        bool swapped = true;
        int start = 0, end = n - 1;

        while (swapped) {
            swapped = false;
            for (int i = start; i < end; i++) {
                if (arr[i].rating > arr[i + 1].rating) {
                    swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;

            swapped = false;
            end--;

            for (int i = end - 1; i >= start; i--) {
                if (arr[i].rating > arr[i + 1].rating) {
                    swap(arr[i], arr[i + 1]);
                    swapped = true;
                }
            }
            start++;
        }

        fromArray(arr, n);
        delete[] arr;
    }

    void imprimir() {
        Node* atual = head;
        cout << "userId movieId rating timestamp" << endl;
        while (atual) {
            cout << atual->data.userId << " "
                 << atual->data.movieId << " "
                 << atual->data.rating << " "
                 << atual->data.timestamp << endl;
            atual = atual->next;
        }
    }

    bool lerCSV(const string& nomeArquivo) {
        ifstream file(nomeArquivo);
        if (!file.is_open()) return false;

        string linha;
        getline(file, linha); // cabeçalho

        while (getline(file, linha)) {
            stringstream ss(linha);
            string temp;
            Rating r;

            getline(ss, temp, ','); r.userId = stoi(temp);
            getline(ss, temp, ','); r.movieId = stoi(temp);
            getline(ss, temp, ','); r.rating = stof(temp);
            getline(ss, temp, ','); r.timestamp = stol(temp);

            enqueue(r);
        }

        file.close();
        return true;
    }

    void liberar() {
        Node* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    ~Fila() {
        liberar();
    }
};

int main() {
    Fila fila;

    if (!fila.lerCSV("ratings.csv")) {
        cout << "Erro ao abrir ratings.csv" << endl;
        return 1;
    }

    cout << "\nAntes da ordenação:\n";
    fila.imprimir();

    fila.cocktail_sort();

    cout << "\nDepois da ordenação (Cocktail Sort por rating):\n";
    fila.imprimir();

    return 0;
}
