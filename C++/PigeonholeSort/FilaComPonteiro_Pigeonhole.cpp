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

    void pigeonhole_sort() {
        int n;
        Rating* arr = toArray(n);

        const float minRating = 0.5;
        const float maxRating = 5.0;
        const int numHoles = static_cast<int>((maxRating - minRating) / 0.5) + 1;

        vector<vector<Rating>> pigeonholes(numHoles);

        // Distribui os elementos nos pigeonholes
        for (int i = 0; i < n; i++) {
            int index = static_cast<int>((arr[i].rating - minRating) / 0.5);
            pigeonholes[index].push_back(arr[i]);
        }

        // Recolhe os elementos dos pigeonholes em ordem
        int idx = 0;
        for (const auto& hole : pigeonholes) {
            for (const auto& r : hole) {
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

    fila.pigeonhole_sort();

    cout << "\nDepois da ordenação (Pigeonhole Sort por rating):\n";
    fila.imprimir();

    return 0;
}
