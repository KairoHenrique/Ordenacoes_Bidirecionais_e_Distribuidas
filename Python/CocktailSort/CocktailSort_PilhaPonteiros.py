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

def cocktail_sort_pilha_ponteiros(pilha: PilhaPonteiros) -> PilhaPonteiros:
    lista = pilha.para_lista()
    n = len(lista)
    if n <= 1:
        return pilha

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

    # Reconstruir pilha com dados ordenados (invertendo para manter ordem correta)
    pilha_ordenada = PilhaPonteiros()
    temp = PilhaPonteiros()
    for item in lista:
        temp.empilhar(item)

    while not temp.esta_vazia():
        pilha_ordenada.empilhar(temp.desempilhar())

    return pilha_ordenada

def escrever_csv(nome_arquivo: str, pilha: PilhaPonteiros):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        # Para manter a ordem correta, transferir para uma pilha temporária
        temp = PilhaPonteiros()
        while not pilha.esta_vazia():
            temp.empilhar(pilha.desempilhar())

        while not temp.esta_vazia():
            item = temp.desempilhar()
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
    dados_ordenados = cocktail_sort_pilha_ponteiros(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_cocktailsort_pilhaponteiros.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
