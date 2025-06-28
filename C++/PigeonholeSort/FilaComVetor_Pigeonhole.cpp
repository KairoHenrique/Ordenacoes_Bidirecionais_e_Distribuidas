#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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

    void imprimir() const {
        cout << "userId movieId rating timestamp\n";
        for (const auto& r : dados) {
            cout << r.userId << " "
                 << r.movieId << " "
                 << r.rating << " "
                 << r.timestamp << "\n";
        }
    }

    void pigeonhole_sort() {
        if (dados.empty()) {
            cout << "Fila vazia. Nada a ordenar.\n";
            return;
        }

        const float minRating = 0.5f;
        const float maxRating = 5.0f;
        const float intervalo = 0.5f;
        const int numHoles = static_cast<int>((maxRating - minRating) / intervalo) + 1;

        vector<vector<Rating>> pigeonholes(numHoles);

        // Distribuição dos elementos nos "buracos"
        for (const auto& r : dados) {
            int index = static_cast<int>((r.rating - minRating) / intervalo);
            pigeonholes[index].push_back(r);
        }

        // Recolher elementos ordenados
        dados.clear();
        for (const auto& hole : pigeonholes) {
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

    cout << "\nAntes do Pigeonhole Sort:\n";
    fila.imprimir();

    fila.pigeonhole_sort();

    cout << "\nDepois do Pigeonhole Sort:\n";
    fila.imprimir();

    return 0;
}
