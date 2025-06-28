#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Rating {
    int userId;
    int movieId;
    float rating;
    long timestamp;
};

class Node {
public:
    Rating data;
    Node* next;
    Node(Rating r) : data(r), next(nullptr) {}
};

class Lista {
private:
    Node* head;

public:
    Lista() : head(nullptr) {}

    ~Lista() {
        liberar();
    }

    void inserirNoInicio(const Rating& r) {
        Node* novo = new Node(r);
        novo->next = head;
        head = novo;
    }

    void liberar() {
        Node* atual = head;
        while (atual) {
            Node* tmp = atual;
            atual = atual->next;
            delete tmp;
        }
        head = nullptr;
    }

    void imprimir() const {
        cout << "userId movieId rating timestamp\n";
        Node* atual = head;
        while (atual) {
            cout << atual->data.userId << " "
                 << atual->data.movieId << " "
                 << atual->data.rating << " "
                 << atual->data.timestamp << "\n";
            atual = atual->next;
        }
    }

    int tamanho() const {
        int count = 0;
        Node* atual = head;
        while (atual) {
            count++;
            atual = atual->next;
        }
        return count;
    }

    void cocktail_sort() {
        if (!head || !head->next) return;

        bool swapped;
        do {
            swapped = false;
            Node* current = head;

            // Passagem da esquerda para a direita
            while (current && current->next) {
                if (current->data.rating > current->next->data.rating) {
                    swap(current->data, current->next->data);
                    swapped = true;
                }
                current = current->next;
            }

            if (!swapped) break;

            swapped = false;
            Node* prev = nullptr;
            current = head;

            // Encontra o último nó
            while (current && current->next) {
                prev = current;
                current = current->next;
            }

            // Passagem da direita para a esquerda
            while (current != head) {
                Node* temp = head;
                while (temp->next != current) {
                    temp = temp->next;
                }
                if (temp->data.rating > current->data.rating) {
                    swap(temp->data, current->data);
                    swapped = true;
                }
                current = temp;
            }

        } while (swapped);
    }

    bool lerCSV(const string& nomeArquivo) {
        ifstream file(nomeArquivo);
        if (!file.is_open()) return false;

        string linha;
        getline(file, linha); // ignora cabeçalho

        while (getline(file, linha)) {
            stringstream ss(linha);
            string temp;
            Rating r;

            getline(ss, temp, ','); r.userId = stoi(temp);
            getline(ss, temp, ','); r.movieId = stoi(temp);
            getline(ss, temp, ','); r.rating = stof(temp);
            getline(ss, temp, ','); r.timestamp = stol(temp);

            inserirNoInicio(r);
        }

        file.close();
        return true;
    }
};

int main() {
    Lista lista;

    if (!lista.lerCSV("ratings.csv")) {
        cout << "Erro ao abrir ratings.csv\n";
        return 1;
    }

    cout << "\nAntes do Cocktail Sort:\n";
    lista.imprimir();

    lista.cocktail_sort();

    cout << "\nDepois do Cocktail Sort:\n";
    lista.imprimir();

    return 0;
}
