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

    void cocktail_sort() {
        if (dados.empty()) return;

        bool swapped = true;
        int start = 0;
        int end = dados.size() - 1;

        while (swapped) {
            swapped = false;

            // Passagem esquerda → direita
            for (int i = start; i < end; ++i) {
                if (dados[i].rating > dados[i + 1].rating) {
                    swap(dados[i], dados[i + 1]);
                    swapped = true;
                }
            }

            // Se nada foi trocado, o vetor está ordenado
            if (!swapped)
                break;

            swapped = false;
            --end;

            // Passagem direita → esquerda
            for (int i = end - 1; i >= start; --i) {
                if (dados[i].rating > dados[i + 1].rating) {
                    swap(dados[i], dados[i + 1]);
                    swapped = true;
                }
            }

            ++start;
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

    cout << "\nAntes do Cocktail Sort:\n";
    lista.imprimir();

    lista.cocktail_sort();

    cout << "\nDepois do Cocktail Sort:\n";
    lista.imprimir();

    return 0;
}
