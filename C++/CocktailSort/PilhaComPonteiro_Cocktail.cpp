#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
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
    Node(Rating r, Node* n = nullptr) : data(r), next(n) {}
};

class Pilha {
private:
    Node* top;

public:
    Pilha() : top(nullptr) {}

    ~Pilha() {
        liberar();
    }

    void push(const Rating& r) {
        Node* novo = new Node(r, top);
        top = novo;
    }

    void liberar() {
        Node* atual = top;
        while (atual) {
            Node* temp = atual;
            atual = atual->next;
            delete temp;
        }
        top = nullptr;
    }

    int tamanho() const {
        int count = 0;
        Node* atual = top;
        while (atual) {
            count++;
            atual = atual->next;
        }
        return count;
    }

    vector<Rating> toVector() const {
        vector<Rating> vec;
        Node* atual = top;
        while (atual) {
            vec.push_back(atual->data);
            atual = atual->next;
        }
        return vec;
    }

    void fromVector(const vector<Rating>& vec) {
        liberar();
        for (int i = vec.size() - 1; i >= 0; --i) {
            push(vec[i]);
        }
    }

    void imprimir() const {
        cout << "userId movieId rating timestamp\n";
        Node* atual = top;
        while (atual) {
            cout << setw(5) << atual->data.userId << " "
                 << setw(7) << atual->data.movieId << " "
                 << fixed << setprecision(1) << atual->data.rating << " "
                 << atual->data.timestamp << "\n";
            atual = atual->next;
        }
    }

    void cocktail_sort() {
        vector<Rating> vec = toVector();
        if (vec.empty()) return;

        bool swapped = true;
        int start = 0;
        int end = vec.size() - 1;

        while (swapped) {
            swapped = false;

            // Esquerda para direita
            for (int i = start; i < end; ++i) {
                if (vec[i].rating > vec[i + 1].rating) {
                    swap(vec[i], vec[i + 1]);
                    swapped = true;
                }
            }

            if (!swapped) break;

            swapped = false;
            --end;

            // Direita para esquerda
            for (int i = end - 1; i >= start; --i) {
                if (vec[i].rating > vec[i + 1].rating) {
                    swap(vec[i], vec[i + 1]);
                    swapped = true;
                }
            }

            ++start;
        }

        fromVector(vec);
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

            push(r);
        }

        file.close();
        return true;
    }
};

int main() {
    Pilha pilha;

    if (!pilha.lerCSV("ratings.csv")) {
        cout << "Erro ao abrir ratings.csv\n";
        return 1;
    }

    cout << "\nAntes do Cocktail Sort:\n";
    pilha.imprimir();

    pilha.cocktail_sort();

    cout << "\nDepois do Cocktail Sort:\n";
    pilha.imprimir();

    return 0;
}
