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

    def trocar_nos(self, no_anterior, no1, no2):
        if no_anterior:
            no_anterior.proximo = no2
        else:
            self.cabeca = no2
        no1.proximo = no2.proximo
        no2.proximo = no1

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

def cocktail_sort_lista(lista: ListaEncadeada):
    if lista.tamanho <= 1:
        return

    swapped = True
    inicio = None
    fim = None

    while swapped:
        swapped = False

        # Passagem da esquerda para a direita
        atual = lista.cabeca
        anterior = None
        anterior_anterior = None

        while atual and atual.proximo != fim:
            if atual.registro['rating'] > atual.proximo.registro['rating']:
                lista.trocar_nos(anterior, atual, atual.proximo)
                swapped = True
                # Atualizar referências após troca
                if anterior:
                    anterior = atual  # O novo nó anterior é o que foi movido para trás
                    atual = anterior.proximo
                else:
                    atual = lista.cabeca.proximo
            else:
                anterior_anterior = anterior
                anterior = atual
                atual = atual.proximo

        if not swapped:
            break

        fim = atual  # Marcar o fim da parte ordenada
        swapped = False

        # Passagem da direita para a esquerda
        while atual and atual != inicio:
            if anterior and atual.registro['rating'] < anterior.registro['rating']:
                lista.trocar_nos(anterior_anterior, anterior, atual)
                swapped = True
                # Atualizar referências após troca
                temp = anterior
                anterior = atual
                atual = temp
            else:
                atual = anterior
                anterior = anterior_anterior
                if anterior_anterior:
                    anterior_anterior = anterior_anterior.proximo if anterior_anterior.proximo != anterior else None

        inicio = atual  # Marcar o início da parte ordenada

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
    cocktail_sort_lista(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_cocktailsort_listaencadeada.csv', dados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {dados.tamanho}")

if __name__ == "__main__":
    main()
