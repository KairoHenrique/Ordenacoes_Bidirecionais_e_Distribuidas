import csv
import time
import sys
from typing import List

MAX_DADOS = 1000000
COLUNA_ORDENACAO = 2

class FilaVetor:
    def __init__(self):
        self.dados = []
        self.inicio = 0
        self.fim = 0
        self.capacidade = 1
        self._redimensionar(self.capacidade)

    def _redimensionar(self, nova_capacidade):
        novos_dados = [None] * nova_capacidade
        for i in range(self.tamanho):
            novos_dados[i] = self.dados[(self.inicio + i) % self.capacidade]
        self.dados = novos_dados
        self.inicio = 0
        self.fim = self.tamanho
        self.capacidade = nova_capacidade

    @property
    def tamanho(self):
        return (self.fim - self.inicio) % self.capacidade if self.fim != self.inicio else 0

    def enfileirar(self, item):
        if self.tamanho == self.capacidade - 1:
            self._redimensionar(2 * self.capacidade)
        self.dados[self.fim] = item
        self.fim = (self.fim + 1) % self.capacidade

    def desenfileirar(self):
        if self.tamanho == 0:
            raise IndexError("Fila vazia")
        item = self.dados[self.inicio]
        self.inicio = (self.inicio + 1) % self.capacidade
        if self.tamanho > 0 and self.tamanho == self.capacidade // 4:
            self._redimensionar(self.capacidade // 2)
        return item

    def esta_vazia(self):
        return self.tamanho == 0

    def para_lista(self):
        lista = []
        for i in range(self.tamanho):
            lista.append(self.dados[(self.inicio + i) % self.capacidade])
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

def pigeonhole_sort_fila(fila: FilaVetor) -> FilaVetor:
    if fila.esta_vazia():
        return FilaVetor()

    lista = fila.para_lista()
    min_rating = 0.5
    max_rating = 5.0
    num_pigeonholes = int((max_rating - min_rating) * 2) + 1

    pigeonholes = [FilaVetor() for _ in range(num_pigeonholes)]

    for item in lista:
        index = int((item['rating'] - min_rating) * 2)
        pigeonholes[index].enfileirar(item)

    resultado = FilaVetor()
    for hole in pigeonholes:
        while not hole.esta_vazia():
            resultado.enfileirar(hole.desenfileirar())

    return resultado

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

    print(f"Ordenando {dados.tamanho} registros...")
    dados_ordenados = pigeonhole_sort_fila(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_pigeonholesort_filavetor.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {dados.tamanho}")

if __name__ == "__main__":
    main()
