#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
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

    void bucket_sort(float bucket_size = 1.0f) {
        vector<Rating> array = toVector();
        if (array.empty()) return;

        float min = array[0].rating, max = array[0].rating;
        for (const Rating& r : array) {
            if (r.rating < min) min = r.rating;
            if (r.rating > max) max = r.rating;
        }

        int bucket_count = static_cast<int>((max - min) / bucket_size) + 1;
        vector<vector<Rating>> buckets(bucket_count);

        for (const Rating& r : array) {
            int index = static_cast<int>((r.rating - min) / bucket_size);
            buckets[index].push_back(r);
        }

        array.clear();
        for (auto& bucket : buckets) {
            sort(bucket.begin(), bucket.end(), [](const Rating& a, const Rating& b) {
                return a.rating < b.rating;
            });
            array.insert(array.end(), bucket.begin(), bucket.end());
        }

        fromVector(array);
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

    cout << "\nAntes do Bucket Sort:\n";
    pilha.imprimir();

    pilha.bucket_sort(1.0f); // Pode ajustar a granularidade

    cout << "\nDepois do Bucket Sort:\n";
    pilha.imprimir();

    return 0;
}
