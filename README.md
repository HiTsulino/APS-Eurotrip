# APS-Eurotrip — Grafo Completo Europeu

Desenvolvida em C++ para representar todas as conexões possíveis entre seis capitais europeias por meio de um grafo completo e não direcionado.

## Capitais

| Vértice | Cidade | País |
|---|---|---|
| 0 | Roma | Itália |
| 1 | Paris | França |
| 2 | Oslo | Noruega |
| 3 | Berna | Suíça |
| 4 | Budapeste | Hungria |
| 5 | Bratislava | Eslováquia |

## Estrutura do grafo

- 6 vértices;
- 15 arestas;
- 5 conexões por vértice;
- Grafo completo e não direcionado.

## Arquivos

| Arquivo | Descrição |
|---|---|
| `Lista ADJ de todos os caminhos.cpp` | Representação do grafo por lista de adjacência. |
| `Todos os caminhos.cpp` | Representação por matriz de adjacência. |
| `Grafo-Caminhos-Custo.cpp` | Matriz de custos em euros (€). |
| `Grafo-Caminhos-Distancia.cpp` | Matriz de distâncias em quilômetros (km). |
| `Grafo-Caminhos-Tempo.cpp` | Matriz de tempos em horas. |

## Critérios e fontes

Os pesos das arestas foram definidos a partir de pesquisas realizadas no [Rome2Rio](https://www.rome2rio.com/), considerando o deslocamento entre as capitais europeias por um único meio de transporte: **carro**.

Foram considerados três critérios:
- Custo estimado da viagem (€).
- Distância do percurso (km).
- Tempo estimado de deslocamento (h).

## Evolução do projeto

A implementação dos caminhos mínimos e das Árvores Geradoras Mínimas (AGM), utilizando os algoritmos de Dijkstra e Prim, está disponível na branch:

**[juntandoCodigos](https://github.com/HiTsulino/APS-Eurotrip/tree/juntandoCodigos)**
