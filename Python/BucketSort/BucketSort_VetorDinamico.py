import csv
import time
import sys
from typing import List, Tuple

MAX_DADOS = 1000000  # Limite máximo de linhas a serem lidas
COLUNA_ORDENACAO = 2  # Padrão: rating (terceira coluna)

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

def bucket_sort(dados: List[Registro]) -> List[Registro]:
    if not dados:
        return []

    # Determinar valor mínimo e máximo
    min_val = min(dado.rating for dado in dados)
    max_val = max(dado.rating for dado in dados)

    # Número de baldes (um para cada valor possível de rating)
    num_buckets = int((max_val - min_val) * 2 + 1)  # 0.5 a 5.0 em passos de 0.5

    # Criar baldes
    baldes = [[] for _ in range(num_buckets)]

    # Distribuir elementos nos baldes
    for dado in dados:
        index = int((dado.rating - min_val) * 2)
        baldes[index].append(dado)

    # Ordenar cada balde (usando insertion sort)
    for balde in baldes:
        for i in range(1, len(balde)):
            chave = balde[i]
            j = i - 1
            while j >= 0 and balde[j].rating > chave.rating:
                balde[j + 1] = balde[j]
                j -= 1
            balde[j + 1] = chave

    # Concatenar baldes
    resultado = []
    for balde in baldes:
        resultado.extend(balde)

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
    dados_ordenados = bucket_sort(dados)

    print("Escrevendo arquivo de saída...")
    escrever_csv('ratings_bucketsort.csv', dados_ordenados)

    tempo_total = time.time() - inicio
    print(f"Concluído! Tempo total: {tempo_total:.2f} segundos")
    print(f"Total de elementos processados: {len(dados)}")

if __name__ == "__main__":
    main()
