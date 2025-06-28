import csv
import time
import sys
from typing import Optional

MAX_DADOS = 1000000
COLUNA_ORDENACAO = 2

class No:
    def __init__(self, registro):
        self.registro = registro
        self.proximo = None

class ListaEncadeada:
    def __init__(self):
        self.cabeca = None
        self.tamanho = 0

    def inserir_no_fim(self, registro):
        novo_no = No(registro)
        if not self.cabeca:
            self.cabeca = novo_no
        else:
            atual = self.cabeca
            while atual.proximo:
                atual = atual.proximo
            atual.proximo = novo_no
        self.tamanho += 1

    def para_lista(self):
        lista = []
        atual = self.cabeca
        while atual:
            lista.append(atual.registro)
            atual = atual.proximo
        return lista

def ler_csv(nome_arquivo: str, max_dados: int) -> ListaEncadeada:
    dados = ListaEncadeada()
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
                    dados.inserir_no_fim(registro)
                except (IndexError, ValueError) as e:
                    print(f"Erro ao processar linha {i}: {e}")
    except FileNotFoundError:
        print(f"Arquivo {nome_arquivo} não encontrado.")
        sys.exit(1)
    return dados

def bucket_sort_lista(lista: ListaEncadeada) -> ListaEncadeada:
    if lista.tamanho == 0:
        return ListaEncadeada()

    # Encontrar mínimo e máximo
    atual = lista.cabeca
    min_val = max_val = atual.registro['rating']
    while atual:
        if atual.registro['rating'] < min_val:
            min_val = atual.registro['rating']
        if atual.registro['rating'] > max_val:
            max_val = atual.registro['rating']
        atual = atual.proximo

    num_buckets = int((max_val - min_val) * 2 + 1)
    baldes = [ListaEncadeada() for _ in range(num_buckets)]

    # Distribuir elementos
    atual = lista.cabeca
    while atual:
        index = int((atual.registro['rating'] - min_val) * 2)
        baldes[index].inserir_no_fim(atual.registro)
        atual = atual.proximo

    # Ordenar cada balde (insertion sort para lista encadeada)
    for balde in baldes:
        if balde.tamanho > 1:
            ordenar_lista_encadeada(balde)

    # Concatenar baldes
    resultado = ListaEncadeada()
    for balde in baldes:
        atual = balde.cabeca
        while atual:
            resultado.inserir_no_fim(atual.registro)
            atual = atual.proximo

    return resultado

def ordenar_lista_encadeada(lista: ListaEncadeada):
    if lista.tamanho <= 1:
        return

    atual = lista.cabeca.proximo
    lista.cabeca.proximo = None

    while atual:
        proximo = atual.proximo
        atual.proximo = None

        if atual.registro['rating'] < lista.cabeca.registro['rating']:
            atual.proximo = lista.cabeca
            lista.cabeca = atual
        else:
            anterior = lista.cabeca
            while anterior.proximo and anterior.proximo.registro['rating'] <= atual.registro['rating']:
                anterior = anterior.proximo
            atual.proximo = anterior.proximo
            anterior.proximo = atual

        atual = proximo

def escrever_csv(nome_arquivo: str, lista: ListaEncadeada):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        atual = lista.cabeca
        while atual:
            escritor.writerow([
                atual.registro['userId'],
                atual.registro['movieId'],
                atual.registro['rating'],
                atual.registro['timestamp']
            ])
            atual = atual.proximo

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if dados.tamanho == 0:
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {dados.tamanho} registros...")
    dados_ordenados = bucket_sort_lista(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_bucketsort_listaencadeada.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {dados.tamanho}")

if __name__ == "__main__":
    main()
