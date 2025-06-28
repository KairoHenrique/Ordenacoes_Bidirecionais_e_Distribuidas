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

class FilaVetor {
private:
    vector<Rating> dados;

public:
    void enqueue(Rating r) {
        dados.push_back(r);
    }

    Rating dequeue() {
        if (dados.empty()) {
            return {-1, -1, -1.0f, -1};
        }
        Rating r = dados.front();
        dados.erase(dados.begin());
        return r;
    }

    void imprimir() const {
        cout << "userId movieId rating timestamp\n";
        for (const auto& r : dados) {
            cout << r.userId << " "
                 << r.movieId << " "
                 << r.rating << " "
                 << r.timestamp << "\n";
        }
    }

    void bucket_sort(float bucket_size) {
        if (dados.empty()) return;

        float min = dados[0].rating, max = dados[0].rating;
        for (const auto& r : dados) {
            if (r.rating < min) min = r.rating;
            if (r.rating > max) max = r.rating;
        }

        int bucket_count = static_cast<int>((max - min) / bucket_size) + 1;
        vector<vector<Rating>> buckets(bucket_count);

        for (const auto& r : dados) {
            int idx = static_cast<int>((r.rating - min) / bucket_size);
            buckets[idx].push_back(r);
        }

        dados.clear();
        for (auto& bucket : buckets) {
            sort(bucket.begin(), bucket.end(), [](const Rating& a, const Rating& b) {
                return a.rating < b.rating;
            });
            dados.insert(dados.end(), bucket.begin(), bucket.end());
        }
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

            enqueue(r);
        }

        file.close();
        return true;
    }
};

int main() {
    FilaVetor fila;

    if (!fila.lerCSV("ratings.csv")) {
        cout << "Erro ao abrir ratings.csv\n";
        return 1;
    }

    cout << "\nAntes do Bucket Sort:\n";
    fila.imprimir();

    fila.bucket_sort(1.0f);

    cout << "\nDepois do Bucket Sort:\n";
    fila.imprimir();

    return 0;
}
