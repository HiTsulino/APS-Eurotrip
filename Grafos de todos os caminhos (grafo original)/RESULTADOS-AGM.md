# Resultados da Árvore Geradora Mínima (AGM) – APS Eurotrip

Algoritmo: **Prim** (matriz de adjacência, O(V²)), partindo de Roma. Código: `Eurotrip-AGM.cpp`.
Vértices: Roma, Paris, Oslo, Berna, Budapeste, Bratislava (grafo completo: 6 vértices, 15 arestas, todos com 5 conexões).

## 1. AGM por custo (rota otimizada geral)

| Conexão selecionada | Custo (€) | Distância (km) | Tempo (h) |
|---|---:|---:|---:|
| Budapeste – Bratislava | 40 | 201 | 2 |
| Berna – Paris | 124 | 528 | 6 |
| Berna – Bratislava | 205 | 883 | 9 |
| Roma – Berna | 207 | 886 | 9 |
| Bratislava – Oslo | 386 | 1755 | 21 |
| **Total (5 arestas = V−1)** | **962** | **4253** | **47** |

## 2. Comparação dos critérios

Cada linha é a árvore obtida otimizando um critério; as colunas mostram quanto essa árvore gasta em cada medida.

| AGM otimizada por | Custo (€) | Distância (km) | Tempo (h) |
|---|---:|---:|---:|
| Custo | **962** | 4253 | 47 |
| Distância | 962 | **4253** | 47 |
| Tempo | 1125 | 4439 | **44** |

## 3. Conclusões

- **Custo e distância geram exatamente a mesma árvore** (as mesmas 5 conexões). Nesse grafo, trechos baratos também são curtos, então otimizar um critério otimiza o outro.
- A árvore por **tempo** troca só uma conexão: usa Paris–Oslo (18 h) no lugar de Bratislava–Oslo (21 h). Ganha **3 h (≈6%)**, mas custa **+163 € (≈17%)** e **+186 km (≈4%)**.
- Portanto **custo (ou distância) foi o critério mais vantajoso**: 3 horas a menos não compensam 163 € a mais, e a árvore é a mais econômica nos dois critérios. O tempo só vale a pena se o grupo tiver pouquíssimos dias.
- **Berna e Bratislava são os hubs** da rede (Berna liga Roma e Paris; Bratislava liga Budapeste e Oslo). **Oslo é o ponto mais caro de incluir**, pois está longe de todos: é a aresta mais cara da árvore em custo e em distância, e a mais lenta.
- Cada AGM é **única** neste grafo (conferido por força bruta sobre todas as combinações de 5 arestas), então o resultado não depende de desempate do algoritmo.
- A AGM mostra **quais conexões** minimizam o custo para ligar todos os destinos. Ela **não é uma sequência de viagem**; a ordem de visita e o trecho Brasil → destino final vêm dos caminhos mínimos (Dijkstra/Floyd-Warshall).
