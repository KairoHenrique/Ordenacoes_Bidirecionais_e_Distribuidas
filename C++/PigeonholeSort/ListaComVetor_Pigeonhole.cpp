#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
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

    void pigeonhole_sort() {
        if (dados.empty()) return;

        int min = floor(dados[0].rating);
        int max = ceil(dados[0].rating);

        for (const auto& r : dados) {
            int val = floor(r.rating);
            if (val < min) min = val;
            if (val > max) max = val;
        }

        int size = max - min + 1;
        vector<vector<Rating>> holes(size);

        for (const auto& r : dados) {
            int index = floor(r.rating) - min;
            holes[index].push_back(r);
        }

        int k = 0;
        for (const auto& hole : holes) {
            for (const auto& r : hole) {
                dados[k++] = r;
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

    cout << "\nAntes do Pigeonhole Sort:\n";
    lista.imprimir();

    lista.pigeonhole_sort();

    cout << "\nDepois do Pigeonhole Sort:\n";
    lista.imprimir();

    return 0;
}
