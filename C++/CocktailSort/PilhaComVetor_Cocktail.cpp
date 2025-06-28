#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
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

    void cocktail_sort() {
        if (dados.size() < 2) return;

        size_t start = 0;
        size_t end = dados.size() - 1;
        bool swapped = true;

        while (swapped) {
            swapped = false;

            for (size_t i = start; i < end; ++i) {
                if (dados[i].rating > dados[i + 1].rating) {
                    swap(dados[i], dados[i + 1]);
                    swapped = true;
                }
            }

            if (!swapped) break;

            swapped = false;
            --end;

            for (size_t i = end; i > start; --i) {
                if (dados[i - 1].rating > dados[i].rating) {
                    swap(dados[i - 1], dados[i]);
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

    cout << "\nAntes do Cocktail Sort:\n";
    pilha.imprimir();

    pilha.cocktail_sort();

    cout << "\nDepois do Cocktail Sort:\n";
    pilha.imprimir();

    return 0;
}
