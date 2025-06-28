import csv
import time
import sys
from typing import List

MAX_DADOS = 1000000
COLUNA_ORDENACAO = 2

class Registro:
    def __init__(self, userId: str, movieId: str, rating: float, timestamp: str):
        self.userId = userId
        self.movieId = movieId
        self.rating = rating
        self.timestamp = timestamp

def ler_csv(nome_arquivo: str, max_dados: int) -> List[Registro]:
    dados = []
    try:
        with open(nome_arquivo, 'r') as arquivo:
            leitor = csv.reader(arquivo)
            next(leitor)  # Pular cabeçalho
            for i, linha in enumerate(leitor):
                if i >= max_dados:
                    break
                try:
                    registro = Registro(
                        userId=linha[0],
                        movieId=linha[1],
                        rating=float(linha[2]),
                        timestamp=linha[3]
                    )
                    dados.append(registro)
                except (IndexError, ValueError) as e:
                    print(f"Erro ao processar linha {i}: {e}")
    except FileNotFoundError:
        print(f"Arquivo {nome_arquivo} não encontrado.")
        sys.exit(1)
    return dados

def cocktail_sort(dados: List[Registro]) -> List[Registro]:
    n = len(dados)
    if n <= 1:
        return dados

    swapped = True
    start = 0
    end = n - 1

    while swapped:
        swapped = False

        # Passagem da esquerda para a direita (como bubble sort)
        for i in range(start, end):
            if dados[i].rating > dados[i + 1].rating:
                dados[i], dados[i + 1] = dados[i + 1], dados[i]
                swapped = True

        if not swapped:
            break

        swapped = False
        end -= 1

        # Passagem da direita para a esquerda
        for i in range(end - 1, start - 1, -1):
            if dados[i].rating > dados[i + 1].rating:
                dados[i], dados[i + 1] = dados[i + 1], dados[i]
                swapped = True

        start += 1

    return dados

def escrever_csv(nome_arquivo: str, dados: List[Registro]):
    with open(nome_arquivo, 'w', newline='') as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(['userId', 'movieId', 'rating', 'timestamp'])
        for registro in dados:
            escritor.writerow([registro.userId, registro.movieId, registro.rating, registro.timestamp])

def main():
    inicio = time.time()

    print("Lendo dados...")
    dados = ler_csv('ratings.csv', MAX_DADOS)
    if not dados:
        print("Nenhum dado foi lido.")
        return

    print(f"Ordenando {len(dados)} registros...")
    dados_ordenados = cocktail_sort(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_cocktailsort.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
