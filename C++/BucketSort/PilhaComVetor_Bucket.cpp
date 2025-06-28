#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

struct Rating {
    int userId;
    int movieId;
    float rating;
    long timestamp;
};

class PilhaVetor {
private:
    vector<Rating> dados;

public:
    void push(const Rating& r) {
        dados.push_back(r);
    }

    Rating pop() {
        if (dados.empty()) {
            return Rating{0, 0, 0.0f, 0};
        }
        Rating r = dados.back();
        dados.pop_back();
        return r;
    }

    void imprimir() const {
        cout << "userId movieId rating timestamp\n";
        for (int i = dados.size() - 1; i >= 0; --i) {
            cout << setw(5) << dados[i].userId << " "
                 << setw(7) << dados[i].movieId << " "
                 << fixed << setprecision(1) << dados[i].rating << " "
                 << dados[i].timestamp << "\n";
        }
    }

    void bucket_sort(float bucket_size = 1.0f) {
        if (dados.empty()) return;

        float min_rating = dados[0].rating;
        float max_rating = dados[0].rating;
        for (const auto& r : dados) {
            if (r.rating < min_rating) min_rating = r.rating;
            if (r.rating > max_rating) max_rating = r.rating;
        }

        int bucket_count = static_cast<int>((max_rating - min_rating) / bucket_size) + 1;
        vector<vector<Rating>> buckets(bucket_count);

        for (const auto& r : dados) {
            int index = static_cast<int>((r.rating - min_rating) / bucket_size);
            buckets[index].push_back(r);
        }

        dados.clear();
        for (auto& bucket : buckets) {
            sort(bucket.begin(), bucket.end(), [](const Rating& a, const Rating& b) {
                return a.rating < b.rating;
            });
            for (const auto& r : bucket) {
                dados.push_back(r);
            }
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

            push(r);
        }

        file.close();
        return true;
    }
};

int main() {
    PilhaVetor pilha;

    if (!pilha.lerCSV("ratings.csv")) {
        cout << "Erro ao abrir ratings.csv\n";
        return 1;
    }

    cout << "\nAntes do Bucket Sort:\n";
    pilha.imprimir();

    pilha.bucket_sort(1.0f); // Você pode ajustar o tamanho do balde

    cout << "\nDepois do Bucket Sort:\n";
    pilha.imprimir();

    return 0;
}
