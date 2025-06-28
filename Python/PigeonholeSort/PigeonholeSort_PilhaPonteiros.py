import csv
import time
import sys
from typing import Optional

MAX_DADOS = 1000000
COLUNA_ORDENACAO = 2

class NoPilha:
    def __init__(self, valor):
        self.valor = valor
        self.abaixo = None

class PilhaPonteiros:
    def __init__(self):
        self.topo = None
        self.tamanho = 0

    def empilhar(self, item):
        novo_no = NoPilha(item)
        novo_no.abaixo = self.topo
        self.topo = novo_no
        self.tamanho += 1

    def desempilhar(self):
        if self.topo is None:
            raise IndexError("Pilha vazia")
        item = self.topo.valor
        self.topo = self.topo.abaixo
        self.tamanho -= 1
        return item

    def __len__(self):
        return self.tamanho

    def esta_vazia(self):
        return self.topo is None

    def para_lista(self):
        lista = []
        atual = self.topo
        while atual:
            lista.append(atual.valor)
            atual = atual.abaixo
        return lista

def ler_csv(nome_arquivo: str, max_dados: int) -> PilhaPonteiros:
    dados = PilhaPonteiros()
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

def pigeonhole_sort_pilha_ponteiros(pilha: PilhaPonteiros) -> PilhaPonteiros:
    if pilha.esta_vazia():
        return PilhaPonteiros()

    lista = pilha.para_lista()
    min_rating = 0.5
    max_rating = 5.0
    num_pigeonholes = int((max_rating - min_rating) * 2) + 1

    pigeonholes = [PilhaPonteiros() for _ in range(num_pigeonholes)]

    for item in lista:
        index = int((item['rating'] - min_rating) * 2)
        pigeonholes[index].empilhar(item)

    # Criar pilha temporária para inverter a ordem
    temp = PilhaPonteiros()
    for hole in pigeonholes:
        while not hole.esta_vazia():
            temp.empilhar(hole.desempilhar())

    # Transferir para pilha resultado (invertendo novamente para ordem correta)
    resultado = PilhaPonteiros()
    while not temp.esta_vazia():
        resultado.empilhar(temp.desempilhar())

    return resultado

def escrever_csv(nome_arquivo: str, pilha: PilhaPonteiros):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        # Desempilhar para lista temporária para manter ordem correta
        temp = []
        while not pilha.esta_vazia():
            temp.append(pilha.desempilhar())
        for item in reversed(temp):
            escritor.writerow([
                item['userId'],
                item['movieId'],
                item['rating'],
                item['timestamp']
            ])

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if dados.esta_vazia():
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = pigeonhole_sort_pilha_ponteiros(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_pigeonholesort_pilhaponteiros.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
