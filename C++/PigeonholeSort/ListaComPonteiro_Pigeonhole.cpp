#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
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

    vector<Rating> paraArray() const {
        vector<Rating> arr;
        Node* atual = head;
        while (atual) {
            arr.push_back(atual->data);
            atual = atual->next;
        }
        return arr;
    }

    void deArray(const vector<Rating>& arr) {
        liberar();
        for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
            inserirNoInicio(*it);
        }
    }

    void pigeonhole_sort() {
        vector<Rating> arr = paraArray();
        if (arr.empty()) return;

        // Determina o intervalo mínimo e máximo de "rating"
        float min_rating = arr[0].rating;
        float max_rating = arr[0].rating;
        for (const auto& r : arr) {
            if (r.rating < min_rating) min_rating = r.rating;
            if (r.rating > max_rating) max_rating = r.rating;
        }

        // Define fator de escala para lidar com valores float como índices inteiros
        const int scale = 10; // Ex: 3.5 * 10 = 35
        int min_idx = static_cast<int>(min_rating * scale);
        int max_idx = static_cast<int>(max_rating * scale);
        int size = max_idx - min_idx + 1;

        vector<vector<Rating>> holes(size);

        // Distribuição dos elementos nos "buracos"
        for (const auto& r : arr) {
            int index = static_cast<int>(r.rating * scale) - min_idx;
            holes[index].push_back(r);
        }

        // Coleta dos dados ordenados
        arr.clear();
        for (const auto& hole : holes) {
            for (const auto& r : hole) {
                arr.push_back(r);
            }
        }

        // Reatualiza a lista encadeada com dados ordenados
        deArray(arr);
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

    cout << "\nAntes do Pigeonhole Sort:\n";
    lista.imprimir();

    lista.pigeonhole_sort();

    cout << "\nDepois do Pigeonhole Sort:\n";
    lista.imprimir();

    return 0;
}
