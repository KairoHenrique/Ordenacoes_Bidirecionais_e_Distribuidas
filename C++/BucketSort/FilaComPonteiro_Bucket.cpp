#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
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

    void bucket_sort() {
        int n;
        Rating* arr = toArray(n);

        // 10 buckets para ratings de 0.5 a 5.0 (intervalos de 0.5)
        const int numBuckets = 10;
        vector<vector<Rating>> buckets(numBuckets);

        // Distribui nos buckets
        for (int i = 0; i < n; i++) {
            int index = min(numBuckets - 1, static_cast<int>((arr[i].rating - 0.5) / 0.5));
            buckets[index].push_back(arr[i]);
        }

        // Ordena cada bucket (insertion sort)
        for (auto& bucket : buckets) {
            for (size_t i = 1; i < bucket.size(); i++) {
                Rating key = bucket[i];
                int j = i - 1;
                while (j >= 0 && bucket[j].rating > key.rating) {
                    bucket[j + 1] = bucket[j];
                    j--;
                }
                bucket[j + 1] = key;
            }
        }

        // Junta os buckets
        int idx = 0;
        for (auto& bucket : buckets) {
            for (auto& r : bucket) {
                arr[idx++] = r;
            }
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

    fila.bucket_sort();

    cout << "\nDepois da ordenação (Bucket Sort por rating):\n";
    fila.imprimir();

    return 0;
}
