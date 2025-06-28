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

def pigeonhole_sort(dados: List[Registro]) -> List[Registro]:
    if not dados:
        return []

    # Converter ratings para índices inteiros (0.5 -> 1, 5.0 -> 10)
    min_rating = 0.5
    max_rating = 5.0
    num_pigeonholes = int((max_rating - min_rating) * 2) + 1

    # Criar pigeonholes
    pigeonholes = [[] for _ in range(num_pigeonholes)]

    # Distribuir os elementos nos pigeonholes
    for registro in dados:
        index = int((registro.rating - min_rating) * 2)
        pigeonholes[index].append(registro)

    # Coletar os elementos ordenados
    resultado = []
    for hole in pigeonholes:
        resultado.extend(hole)

    return resultado

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
    dados_ordenados = pigeonhole_sort(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_pigeonholesort.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
