import csv
import time
import sys
from typing import Optional

MAX_DADOS = 1000000
COLUNA_ORDENACAO = 2

class NoFila:
    def __init__(self, valor):
        self.valor = valor
        self.proximo = None

class FilaPonteiros:
    def __init__(self):
        self.frente = None
        self.final = None
        self.tamanho = 0

    def enfileirar(self, item):
        novo_no = NoFila(item)
        if self.final is None:
            self.frente = self.final = novo_no
        else:
            self.final.proximo = novo_no
            self.final = novo_no
        self.tamanho += 1

    def desenfileirar(self):
        if self.frente is None:
            raise IndexError("Fila vazia")
        item = self.frente.valor
        self.frente = self.frente.proximo
        if self.frente is None:
            self.final = None
        self.tamanho -= 1
        return item

    def __len__(self):
        return self.tamanho

    def esta_vazia(self):
        return self.frente is None

    def para_lista(self):
        lista = []
        atual = self.frente
        while atual:
            lista.append(atual.valor)
            atual = atual.proximo
        return lista

def ler_csv(nome_arquivo: str, max_dados: int) -> FilaPonteiros:
    dados = FilaPonteiros()
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

def cocktail_sort_fila_ponteiros(fila: FilaPonteiros) -> FilaPonteiros:
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
    fila_ordenada = FilaPonteiros()
    for item in lista:
        fila_ordenada.enfileirar(item)

    return fila_ordenada

def escrever_csv(nome_arquivo: str, fila: FilaPonteiros):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        atual = fila.frente
        while atual:
            escritor.writerow([
                atual.valor['userId'],
                atual.valor['movieId'],
                atual.valor['rating'],
                atual.valor['timestamp']
            ])
            atual = atual.proximo

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if dados.esta_vazia():
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = cocktail_sort_fila_ponteiros(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_cocktailsort_filaponteiros.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
