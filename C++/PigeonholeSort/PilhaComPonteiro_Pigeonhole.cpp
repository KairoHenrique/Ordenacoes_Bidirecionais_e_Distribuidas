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

    void pigeonhole_sort() {
        vector<Rating> array = toVector();
        if (array.empty()) return;

        int min = floor(array[0].rating);
        int max = ceil(array[0].rating);

        for (const Rating& r : array) {
            int val = floor(r.rating);
            if (val < min) min = val;
            if (val > max) max = val;
        }

        int size = max - min + 1;
        vector<vector<Rating>> holes(size);

        for (const Rating& r : array) {
            int index = floor(r.rating) - min;
            holes[index].push_back(r);
        }

        vector<Rating> ordenado;
        for (const auto& hole : holes) {
            for (const Rating& r : hole) {
                ordenado.push_back(r);
            }
        }

        fromVector(ordenado);
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

    cout << "\nAntes do Pigeonhole Sort:\n";
    pilha.imprimir();

    pilha.pigeonhole_sort();

    cout << "\nDepois do Pigeonhole Sort:\n";
    pilha.imprimir();

    return 0;
}
