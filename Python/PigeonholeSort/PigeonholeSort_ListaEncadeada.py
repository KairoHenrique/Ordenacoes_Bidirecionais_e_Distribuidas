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

def pigeonhole_sort_lista(lista: ListaEncadeada) -> ListaEncadeada:
    if lista.tamanho == 0:
        return ListaEncadeada()

    # Converter ratings para índices
    min_rating = 0.5
    max_rating = 5.0
    num_pigeonholes = int((max_rating - min_rating) * 2) + 1

    # Criar pigeonholes como listas encadeadas
    pigeonholes = [ListaEncadeada() for _ in range(num_pigeonholes)]

    # Distribuir os elementos
    atual = lista.cabeca
    while atual:
        index = int((atual.registro['rating'] - min_rating) * 2)
        pigeonholes[index].inserir_no_fim(atual.registro)
        atual = atual.proximo

    # Coletar os elementos ordenados
    resultado = ListaEncadeada()
    for hole in pigeonholes:
        atual_hole = hole.cabeca
        while atual_hole:
            resultado.inserir_no_fim(atual_hole.registro)
            atual_hole = atual_hole.proximo

    return resultado

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
    dados_ordenados = pigeonhole_sort_lista(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_pigeonholesort_listaencadeada.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {dados.tamanho}")

if __name__ == "__main__":
    main()
