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

def pigeonhole_sort_pilha(pilha: PilhaVetor) -> PilhaVetor:
    if pilha.esta_vazia():
        return PilhaVetor()

    lista = pilha.para_lista()
    min_rating = 0.5
    max_rating = 5.0
    num_pigeonholes = int((max_rating - min_rating) * 2) + 1

    pigeonholes = [PilhaVetor() for _ in range(num_pigeonholes)]

    for item in lista:
        index = int((item['rating'] - min_rating) * 2)
        pigeonholes[index].empilhar(item)

    # Criar pilha temporária para inverter a ordem
    temp = PilhaVetor()
    for hole in pigeonholes:
        while not hole.esta_vazia():
            temp.empilhar(hole.desempilhar())

    # Transferir para pilha resultado (invertendo novamente para ordem correta)
    resultado = PilhaVetor()
    while not temp.esta_vazia():
        resultado.empilhar(temp.desempilhar())

    return resultado

def escrever_csv(nome_arquivo: str, pilha: PilhaVetor):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        # Desempilhar para lista temporária para manter ordem correta
        temp = []
        while not pilha.esta_vazia():
            temp.append(pilha.desempilhar())
        for item in reversed(temp):
            escritor.writerow([item['userId'], item['movieId'], item['rating'], item['timestamp']])

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if dados.esta_vazia():
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = pigeonhole_sort_pilha(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_pigeonholesort_pilhavetor.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
