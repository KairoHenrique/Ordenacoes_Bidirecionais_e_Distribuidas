import csv
import time
import sys
from typing import List

MAX_DADOS = 1000000
COLUNA_ORDENACAO = 2

class FilaVetor:
    def __init__(self):
        self.dados = []
        self.tamanho = 0
        self.capacidade = 1
        self.inicio = 0

    def enfileirar(self, item):
        if self.tamanho == self.capacidade:
            self._redimensionar(2 * self.capacidade)
        pos = (self.inicio + self.tamanho) % self.capacidade
        if len(self.dados) < self.capacidade:
            self.dados.append(item)
        else:
            self.dados[pos] = item
        self.tamanho += 1

    def desenfileirar(self):
        if self.tamanho == 0:
            raise IndexError("Fila vazia")
        item = self.dados[self.inicio]
        self.inicio = (self.inicio + 1) % self.capacidade
        self.tamanho -= 1
        if 0 < self.tamanho <= self.capacidade // 4:
            self._redimensionar(self.capacidade // 2)
        return item

    def _redimensionar(self, nova_capacidade):
        novos_dados = []
        for i in range(self.tamanho):
            pos = (self.inicio + i) % self.capacidade
            novos_dados.append(self.dados[pos])
        self.dados = novos_dados
        self.inicio = 0
        self.capacidade = nova_capacidade

    def __len__(self):
        return self.tamanho

    def esta_vazia(self):
        return self.tamanho == 0

    def para_lista(self):
        lista = []
        for i in range(self.tamanho):
            pos = (self.inicio + i) % self.capacidade
            lista.append(self.dados[pos])
        return lista

def ler_csv(nome_arquivo: str, max_dados: int) -> FilaVetor:
    dados = FilaVetor()
    try:
        with open(nome_arquivo, 'r') as arquivo:
            leitor = csv.reader(arquivo)
            next(leitor)
            for i, linha in enumerate(leitor):
                if i >= max_dados:
                    break
                try:
                    registro = {
                        'userId': linha[0],
                        'movieId': linha[1],
                        'rating': float(linha[2]),
                        'timestamp': linha[3]
                    }
                    dados.enfileirar(registro)
                except (IndexError, ValueError) as e:
                    print(f"Erro ao processar linha {i}: {e}")
    except FileNotFoundError:
        print(f"Arquivo {nome_arquivo} não encontrado.")
        sys.exit(1)
    return dados

def cocktail_sort_fila(fila: FilaVetor) -> FilaVetor:
    lista = fila.para_lista()
    n = len(lista)
    if n <= 1:
        return fila

    swapped = True
    start = 0
    end = n - 1

    while swapped:
        swapped = False

        # Passagem da esquerda para a direita
        for i in range(start, end):
            if lista[i]['rating'] > lista[i + 1]['rating']:
                lista[i], lista[i + 1] = lista[i + 1], lista[i]
                swapped = True

        if not swapped:
            break

        swapped = False
        end -= 1

        # Passagem da direita para a esquerda
        for i in range(end - 1, start - 1, -1):
            if lista[i]['rating'] > lista[i + 1]['rating']:
                lista[i], lista[i + 1] = lista[i + 1], lista[i]
                swapped = True

        start += 1

    # Reconstruir fila com dados ordenados
    fila_ordenada = FilaVetor()
    for item in lista:
        fila_ordenada.enfileirar(item)

    return fila_ordenada

def escrever_csv(nome_arquivo: str, fila: FilaVetor):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        for item in fila.para_lista():
            escritor.writerow([item['userId'], item['movieId'], item['rating'], item['timestamp']])

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if dados.esta_vazia():
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = cocktail_sort_fila(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_cocktailsort_filavetor.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
