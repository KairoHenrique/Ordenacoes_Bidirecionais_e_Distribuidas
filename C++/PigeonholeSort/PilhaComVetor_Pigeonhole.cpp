#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
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

    void pigeonhole_sort() {
        if (dados.empty()) return;

        // Determina o intervalo de valores inteiros de rating
        int min_rating = floor(dados[0].rating);
        int max_rating = ceil(dados[0].rating);

        for (const auto& r : dados) {
            int val = floor(r.rating);
            if (val < min_rating) min_rating = val;
            if (val > max_rating) max_rating = val;
        }

        int size = max_rating - min_rating + 1;
        vector<vector<Rating>> holes(size);

        for (const auto& r : dados) {
            int index = floor(r.rating) - min_rating;
            holes[index].push_back(r);
        }

        // Junta os ratings ordenados de volta na pilha (vetor)
        dados.clear();
        for (const auto& hole : holes) {
            for (const auto& r : hole) {
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

    cout << "\nAntes do Pigeonhole Sort:\n";
    pilha.imprimir();

    pilha.pigeonhole_sort();

    cout << "\nDepois do Pigeonhole Sort:\n";
    pilha.imprimir();

    return 0;
}
