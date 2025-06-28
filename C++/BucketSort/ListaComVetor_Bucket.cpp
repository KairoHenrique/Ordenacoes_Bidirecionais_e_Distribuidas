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

class ListaVetor {
private:
    vector<Rating> dados;

public:
    void adicionar(const Rating& r) {
        dados.push_back(r);
    }

    void imprimir() const {
        cout << "userId movieId rating timestamp\n";
        for (const auto& r : dados) {
            cout << setw(5) << r.userId << " "
                 << setw(7) << r.movieId << " "
                 << fixed << setprecision(1) << r.rating << " "
                 << r.timestamp << "\n";
        }
    }

    int tamanho() const {
        return dados.size();
    }

    void bucket_sort(float bucket_size = 1.0f) {
        if (dados.empty()) return;

        // Descobrir mínimo e máximo
        float min = dados[0].rating, max = dados[0].rating;
        for (const auto& r : dados) {
            if (r.rating < min) min = r.rating;
            if (r.rating > max) max = r.rating;
        }

        // Número de baldes
        int bucket_count = static_cast<int>((max - min) / bucket_size) + 1;
        vector<vector<Rating>> buckets(bucket_count);

        // Distribuir os elementos nos baldes
        for (const auto& r : dados) {
            int index = static_cast<int>((r.rating - min) / bucket_size);
            buckets[index].push_back(r);
        }

        // Ordenar cada balde e concatenar
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

            adicionar(r);
        }

        file.close();
        return true;
    }
};

int main() {
    ListaVetor lista;

    if (!lista.lerCSV("ratings.csv")) {
        cout << "Erro ao abrir ratings.csv\n";
        return 1;
    }

    cout << "\nAntes do Bucket Sort:\n";
    lista.imprimir();

    lista.bucket_sort(1.0f); // você pode mudar o tamanho do balde aqui

    cout << "\nDepois do Bucket Sort:\n";
    lista.imprimir();

    return 0;
}
