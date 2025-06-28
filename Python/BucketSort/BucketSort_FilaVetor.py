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

def bucket_sort_fila(fila: FilaVetor) -> FilaVetor:
    if len(fila) == 0:
        return FilaVetor()

    lista = fila.para_lista()
    min_val = min(item['rating'] for item in lista)
    max_val = max(item['rating'] for item in lista)

    num_buckets = int((max_val - min_val) * 2 + 1)
    baldes = [FilaVetor() for _ in range(num_buckets)]

    for item in lista:
        index = int((item['rating'] - min_val) * 2)
        baldes[index].enfileirar(item)

    for balde in baldes:
        if len(balde) > 1:
            ordenar_fila(balde)

    resultado = FilaVetor()
    for balde in baldes:
        while len(balde) > 0:
            resultado.enfileirar(balde.desenfileirar())

    return resultado

def ordenar_fila(fila: FilaVetor):
    lista = fila.para_lista()
    lista.sort(key=lambda x: x['rating'])
    fila.dados = lista
    fila.tamanho = len(lista)
    fila.inicio = 0
    fila.capacidade = max(1, len(lista))

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
    if len(dados) == 0:
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = bucket_sort_fila(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_bucketsort_filavetor.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
