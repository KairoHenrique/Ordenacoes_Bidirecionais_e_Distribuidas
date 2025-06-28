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

    void cocktail_sort() {
        if (dados.empty()) {
            cout << "Fila vazia. Nada a ordenar.\n";
            return;
        }

        bool swapped = true;
        int start = 0;
        int end = dados.size() - 1;

        while (swapped) {
            swapped = false;

            for (int i = start; i < end; i++) {
                if (dados[i].rating > dados[i + 1].rating) {
                    swap(dados[i], dados[i + 1]);
                    swapped = true;
                }
            }

            if (!swapped) break;

            swapped = false;
            end--;

            for (int i = end - 1; i >= start; i--) {
                if (dados[i].rating > dados[i + 1].rating) {
                    swap(dados[i], dados[i + 1]);
                    swapped = true;
                }
            }
            start++;
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

    cout << "\nAntes do Cocktail Sort:\n";
    fila.imprimir();

    fila.cocktail_sort();

    cout << "\nDepois do Cocktail Sort:\n";
    fila.imprimir();

    return 0;
}
