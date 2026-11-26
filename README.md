# APS-Eurotrip

Projeto de Estrutura de Dados Não Lineares que compara rotas por custo, distância e tempo e calcula a árvore geradora mínima (AGM) entre as cidades.

## Versões do programa

- [Versão São Paulo](versao-sao-paulo/) — a viagem começa em São Paulo (GRU) e o usuário escolhe a capital europeia de destino.

- [Versão Europa](versao-europa/) — o usuário escolhe a origem e o destino entre seis capitais europeias.

Cada versão tem quatro arquivos, organizados da mesma forma nas duas pastas.

| Arquivo | Função |
|---|---|
| `grafo.cpp` | Define os tipos que ficavam em `eurotrip.hpp`, incluindo `Pesos`, `Aresta`, `Trecho` e `Grafo`. Cadastra as cidades e ligações e verifica a conectividade. |
| `algoritmos.cpp` | Calcula as rotas com Dijkstra, as árvores geradoras mínimas com Prim e os totais de cada resultado. |
| `menu.cpp` | Lê as escolhas do usuário e apresenta as rotas, as matrizes e a comparação das árvores. |
| `main.cpp` | Inclui os três arquivos na ordem acima e inicia a versão escolhida. |

Para executar, abra o terminal na pasta da versão escolhida e compile somente `main.cpp` com C++17. Os três arquivos incluídos por ele entram na mesma compilação.

```powershell
g++ -std=c++17 main.cpp -o eurotrip.exe
.\eurotrip.exe
```
