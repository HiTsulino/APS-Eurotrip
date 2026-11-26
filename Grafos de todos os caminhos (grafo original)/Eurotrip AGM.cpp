// ============================================================
// APS Eurotrip - Arvore Geradora Minima (AGM) com algoritmo de PRIM
// Estrutura de Dados Nao Lineares
//
// Este arquivo reune:
//   1) o grafo ponderado em MATRIZ DE ADJACENCIA (mesma classe dos
//      arquivos Grafo-Caminhos-Custo/Distancia/Tempo.cpp);
//   2) a funcao prim()  -> calcula a AGM;
//   3) a exibicao dos resultados: arestas escolhidas, custo total,
//      distancia total e tempo total;
//   4) a comparacao entre os tres criterios (custo, distancia, tempo).
//
// Vertices: 0 Roma | 1 Paris | 2 Oslo | 3 Berna | 4 Budapeste | 5 Bratislava
// ============================================================

#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
#include <string>

using namespace std;

// ------------------------------------------------------------
// Grafo nao direcionado e ponderado (matriz de adjacencia)
// INT_MAX significa "nao existe aresta"
// ------------------------------------------------------------
class GrafoMatrizPonderado {
public:
    int numVertices;
    vector<vector<int>> matrizAdj;

    GrafoMatrizPonderado(int vertices)
        : numVertices(vertices),
          matrizAdj(vertices, vector<int>(vertices, INT_MAX)) {
        for (int i = 0; i < vertices; i++) matrizAdj[i][i] = 0;
    }

    void adicionarAresta(int origem, int destino, int peso) {
        if (origem >= 0 && origem < numVertices &&
            destino >= 0 && destino < numVertices) {
            matrizAdj[origem][destino] = peso;
            matrizAdj[destino][origem] = peso;   // nao direcionado
        } else {
            cout << "Erro: vertices invalidos" << endl;
        }
    }

    int obterPeso(int origem, int destino) const {
        if (origem >= 0 && origem < numVertices &&
            destino >= 0 && destino < numVertices) {
            return matrizAdj[origem][destino];
        }
        return INT_MAX;
    }

    int obterNumVertices() const { return numVertices; }

    void imprimir(const vector<string>& nomes) const {
        cout << setw(11) << " ";
        for (int i = 0; i < numVertices; i++) cout << setw(11) << nomes[i];
        cout << endl;
        for (int i = 0; i < numVertices; i++) {
            cout << setw(10) << nomes[i] << "|";
            for (int j = 0; j < numVertices; j++) {
                if (matrizAdj[i][j] == INT_MAX) cout << setw(11) << "-";
                else                            cout << setw(11) << matrizAdj[i][j];
            }
            cout << endl;
        }
    }
};

// ------------------------------------------------------------
// Resultado da AGM
// ------------------------------------------------------------
struct ArestaAGM {
    int u, v, peso;
};

struct ResultadoAGM {
    vector<ArestaAGM> arestas;  // arestas escolhidas (V-1 se o grafo for conexo)
    long long total = 0;        // soma dos pesos das arestas escolhidas
    bool conexo = true;         // false se alguma cidade ficou inalcancavel
};

// ------------------------------------------------------------
// ALGORITMO DE PRIM
// Ideia: comeca em um vertice e, a cada passo, adiciona a aresta de
// MENOR peso que liga a arvore ja construida a um vertice fora dela.
//
//   custoMin[v] = menor peso conhecido de uma aresta ligando v a arvore
//   pai[v]      = vertice da arvore que da esse menor peso
//   naArvore[v] = true se v ja foi incluido
//
// Complexidade com matriz: O(V^2)
// ------------------------------------------------------------
ResultadoAGM prim(const GrafoMatrizPonderado& g, int inicio = 0) {
    int n = g.obterNumVertices();
    ResultadoAGM res;

    vector<int>  custoMin(n, INT_MAX);
    vector<int>  pai(n, -1);
    vector<bool> naArvore(n, false);

    custoMin[inicio] = 0;

    for (int passo = 0; passo < n; passo++) {
        // 1) escolhe o vertice fora da arvore com menor custoMin
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!naArvore[v] && (u == -1 || custoMin[v] < custoMin[u])) u = v;
        }

        // nenhum vertice alcancavel restante -> grafo desconexo
        if (custoMin[u] == INT_MAX) { res.conexo = false; break; }

        // 2) inclui u na arvore e registra a aresta (pai[u], u)
        naArvore[u] = true;
        if (pai[u] != -1) {
            res.arestas.push_back({pai[u], u, custoMin[u]});
            res.total += custoMin[u];
        }

        // 3) atualiza os vizinhos de u que ainda estao fora da arvore
        for (int v = 0; v < n; v++) {
            int w = g.obterPeso(u, v);
            if (!naArvore[v] && w != INT_MAX && w < custoMin[v]) {
                custoMin[v] = w;
                pai[v] = u;
            }
        }
    }
    return res;
}

// ------------------------------------------------------------
// Soma, usando OUTRO grafo, os pesos das arestas de uma AGM.
// Serve para medir "quanto a arvore escolhida por custo gasta de
// distancia e de tempo" e vice-versa.
// ------------------------------------------------------------
long long somarComOutroPeso(const ResultadoAGM& agm, const GrafoMatrizPonderado& g) {
    long long soma = 0;
    for (const auto& a : agm.arestas) soma += g.obterPeso(a.u, a.v);
    return soma;
}

// ------------------------------------------------------------
// Dados do grafo (iguais aos arquivos Grafo-Caminhos-*.cpp)
// ------------------------------------------------------------
const int NUM_CIDADES = 6;
const vector<string> NOMES = {"Roma", "Paris", "Oslo", "Berna", "Budapeste", "Bratislava"};

struct DadoAresta { int u, v, custo, distancia, tempo; };

const vector<DadoAresta> ARESTAS = {
    // Roma
    {0, 1,  311, 1415, 17},
    {0, 2,  758, 2689, 25},
    {0, 3,  207,  886,  9},
    {0, 4,  208, 1162, 12},
    {0, 5,  246, 1114, 11},
    // Paris
    {1, 2,  549, 1941, 18},
    {1, 3,  124,  528,  6},
    {1, 4,  289, 1485, 13},
    {1, 5,  290, 1290, 12},
    // Oslo
    {2, 3,  411, 1866, 23},
    {2, 4,  617, 2179, 21},
    {2, 5,  386, 1755, 21},
    // Berna
    {3, 4,  248, 1060, 10},
    {3, 5,  205,  883,  9},
    // Budapeste
    {4, 5,   40,  201,  2}
};

void imprimirAGM(const string& titulo, const ResultadoAGM& agm,
                 const GrafoMatrizPonderado& gCusto,
                 const GrafoMatrizPonderado& gDist,
                 const GrafoMatrizPonderado& gTempo,
                 const string& unidade) {
    cout << "=== " << titulo << " ===" << endl;
    cout << left << setw(24) << "Conexao" << right
         << setw(12) << "Custo(EUR)" << setw(12) << "Dist(km)" << setw(12) << "Tempo(h)" << endl;
    for (const auto& a : agm.arestas) {
        cout << left << setw(24) << (NOMES[a.u] + " - " + NOMES[a.v]) << right
             << setw(12) << gCusto.obterPeso(a.u, a.v)
             << setw(12) << gDist.obterPeso(a.u, a.v)
             << setw(12) << gTempo.obterPeso(a.u, a.v) << endl;
    }
    cout << left << setw(24) << "TOTAL" << right
         << setw(12) << somarComOutroPeso(agm, gCusto)
         << setw(12) << somarComOutroPeso(agm, gDist)
         << setw(12) << somarComOutroPeso(agm, gTempo) << endl;
    cout << "Total otimizado (" << unidade << "): " << agm.total
         << " | arestas: " << agm.arestas.size()
         << " | grafo conexo: " << (agm.conexo ? "sim" : "nao") << endl << endl;
}

int main() {
    // 1) monta os tres grafos (um por criterio)
    GrafoMatrizPonderado gCusto(NUM_CIDADES), gDist(NUM_CIDADES), gTempo(NUM_CIDADES);
    for (const auto& a : ARESTAS) {
        gCusto.adicionarAresta(a.u, a.v, a.custo);
        gDist.adicionarAresta(a.u, a.v, a.distancia);
        gTempo.adicionarAresta(a.u, a.v, a.tempo);
    }

    // 2) matrizes de adjacencia ponderadas
    cout << "Matriz de CUSTO (EUR)" << endl;     gCusto.imprimir(NOMES); cout << endl;
    cout << "Matriz de DISTANCIA (km)" << endl;  gDist.imprimir(NOMES);  cout << endl;
    cout << "Matriz de TEMPO (horas)" << endl;   gTempo.imprimir(NOMES); cout << endl;

    // 3) AGM (Prim) para cada criterio
    ResultadoAGM agmCusto = prim(gCusto);
    ResultadoAGM agmDist  = prim(gDist);
    ResultadoAGM agmTempo = prim(gTempo);

    imprimirAGM("AGM por CUSTO (rota otimizada geral)", agmCusto, gCusto, gDist, gTempo, "EUR");
    imprimirAGM("AGM por DISTANCIA", agmDist, gCusto, gDist, gTempo, "km");
    imprimirAGM("AGM por TEMPO", agmTempo, gCusto, gDist, gTempo, "h");

    // 4) comparacao: cada linha = arvore escolhida por um criterio,
    //    colunas = quanto essa arvore gasta em cada medida
    cout << "=== COMPARACAO DOS CRITERIOS ===" << endl;
    cout << left << setw(22) << "AGM otimizada por" << right
         << setw(12) << "Custo(EUR)" << setw(12) << "Dist(km)" << setw(12) << "Tempo(h)" << endl;
    cout << left << setw(22) << "Custo" << right
         << setw(12) << somarComOutroPeso(agmCusto, gCusto)
         << setw(12) << somarComOutroPeso(agmCusto, gDist)
         << setw(12) << somarComOutroPeso(agmCusto, gTempo) << endl;
    cout << left << setw(22) << "Distancia" << right
         << setw(12) << somarComOutroPeso(agmDist, gCusto)
         << setw(12) << somarComOutroPeso(agmDist, gDist)
         << setw(12) << somarComOutroPeso(agmDist, gTempo) << endl;
    cout << left << setw(22) << "Tempo" << right
         << setw(12) << somarComOutroPeso(agmTempo, gCusto)
         << setw(12) << somarComOutroPeso(agmTempo, gDist)
         << setw(12) << somarComOutroPeso(agmTempo, gTempo) << endl;

    return 0;
}