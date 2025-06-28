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

def bucket_sort_pilha_ponteiros(pilha: PilhaPonteiros) -> PilhaPonteiros:
    if pilha.esta_vazia():
        return PilhaPonteiros()

    lista = pilha.para_lista()
    min_val = min(item['rating'] for item in lista)
    max_val = max(item['rating'] for item in lista)

    num_buckets = int((max_val - min_val) * 2 + 1)
    baldes = [PilhaPonteiros() for _ in range(num_buckets)]

    for item in lista:
        index = int((item['rating'] - min_val) * 2)
        baldes[index].empilhar(item)

    for balde in baldes:
        if len(balde) > 1:
            ordenar_pilha_ponteiros(balde)

    resultado = PilhaPonteiros()
    # Transferir para pilha temporária para inverter a ordem
    temp = PilhaPonteiros()
    for balde in baldes:
        while not balde.esta_vazia():
            temp.empilhar(balde.desempilhar())

    while not temp.esta_vazia():
        resultado.empilhar(temp.desempilhar())

    return resultado

def ordenar_pilha_ponteiros(pilha: PilhaPonteiros):
    lista = pilha.para_lista()
    lista.sort(key=lambda x: x['rating'])
    pilha.topo = None
    pilha.tamanho = 0
    for item in reversed(lista):  # Empilhar em ordem crescente
        pilha.empilhar(item)

def escrever_csv(nome_arquivo: str, pilha: PilhaPonteiros):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        atual = pilha.topo
        while atual:
            escritor.writerow([
                atual.valor['userId'],
                atual.valor['movieId'],
                atual.valor['rating'],
                atual.valor['timestamp']
            ])
            atual = atual.abaixo

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if dados.esta_vazia():
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = bucket_sort_pilha_ponteiros(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_bucketsort_pilhaponteiros.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
