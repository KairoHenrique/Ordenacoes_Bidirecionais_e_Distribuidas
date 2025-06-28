import csv
import time
import sys
from typing import List

MAX_DADOS = 1000000
COLUNA_ORDENACAO = 2

class PilhaVetor:
    def __init__(self):
        self.dados = []
        self.topo = -1
        self.capacidade = 1

    def empilhar(self, item):
        if self.topo == self.capacidade - 1:
            self._redimensionar(2 * self.capacidade)
        self.topo += 1
        if len(self.dados) <= self.topo:
            self.dados.append(item)
        else:
            self.dados[self.topo] = item

    def desempilhar(self):
        if self.topo == -1:
            raise IndexError("Pilha vazia")
        item = self.dados[self.topo]
        self.topo -= 1
        if 0 < self.topo + 1 <= self.capacidade // 4:
            self._redimensionar(self.capacidade // 2)
        return item

    def _redimensionar(self, nova_capacidade):
        self.dados = self.dados[:self.topo + 1]
        self.capacidade = nova_capacidade

    def __len__(self):
        return self.topo + 1

    def esta_vazia(self):
        return self.topo == -1

    def para_lista(self):
        return self.dados[:self.topo + 1]

def ler_csv(nome_arquivo: str, max_dados: int) -> PilhaVetor:
    dados = PilhaVetor()
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
                    dados.empilhar(registro)
                except (IndexError, ValueError) as e:
                    print(f"Erro ao processar linha {i}: {e}")
    except FileNotFoundError:
        print(f"Arquivo {nome_arquivo} não encontrado.")
        sys.exit(1)
    return dados

def bucket_sort_pilha(pilha: PilhaVetor) -> PilhaVetor:
    if pilha.esta_vazia():
        return PilhaVetor()

    lista = pilha.para_lista()
    min_val = min(item['rating'] for item in lista)
    max_val = max(item['rating'] for item in lista)

    num_buckets = int((max_val - min_val) * 2 + 1)
    baldes = [PilhaVetor() for _ in range(num_buckets)]

    for item in lista:
        index = int((item['rating'] - min_val) * 2)
        baldes[index].empilhar(item)

    for balde in baldes:
        if len(balde) > 1:
            ordenar_pilha(balde)

    resultado = PilhaVetor()
    # Transferir para pilha temporária para inverter a ordem
    temp = PilhaVetor()
    for balde in baldes:
        while not balde.esta_vazia():
            temp.empilhar(balde.desempilhar())

    while not temp.esta_vazia():
        resultado.empilhar(temp.desempilhar())

    return resultado

def ordenar_pilha(pilha: PilhaVetor):
    lista = pilha.para_lista()
    lista.sort(key=lambda x: x['rating'])
    pilha.topo = -1
    for item in lista:
        pilha.empilhar(item)

def escrever_csv(nome_arquivo: str, pilha: PilhaVetor):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        for item in reversed(pilha.para_lista()):  # Reverter para ordem correta
            escritor.writerow([item['userId'], item['movieId'], item['rating'], item['timestamp']])

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if dados.esta_vazia():
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = bucket_sort_pilha(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_bucketsort_pilhavetor.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
