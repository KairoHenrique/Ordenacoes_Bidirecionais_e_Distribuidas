#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
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

    void bucket_sort(float bucket_size) {
        vector<Rating> array = paraArray();
        if (array.empty()) return;

        float min = array[0].rating, max = array[0].rating;
        for (const auto& r : array) {
            if (r.rating < min) min = r.rating;
            if (r.rating > max) max = r.rating;
        }

        int bucket_count = static_cast<int>((max - min) / bucket_size) + 1;
        vector<vector<Rating>> buckets(bucket_count);

        for (const auto& r : array) {
            int idx = static_cast<int>((r.rating - min) / bucket_size);
            buckets[idx].push_back(r);
        }

        array.clear();
        for (auto& bucket : buckets) {
            sort(bucket.begin(), bucket.end(), [](const Rating& a, const Rating& b) {
                return a.rating < b.rating;
            });
            array.insert(array.end(), bucket.begin(), bucket.end());
        }

        deArray(array);
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

    cout << "\nAntes do Bucket Sort:\n";
    lista.imprimir();

    lista.bucket_sort(1.0f);

    cout << "\nDepois do Bucket Sort:\n";
    lista.imprimir();

    return 0;
}
