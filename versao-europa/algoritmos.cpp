#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

// Caminhos mínimos de Nicolas e árvore geradora mínima de Viviane.

// Seleciona a medida que será minimizada.
double valor(Pesos p, Criterio c) {
    if (c == Criterio::Custo) return p.euros;
    if (c == Criterio::Distancia) return p.km;
    return p.horas;
}

// Encontra o menor caminho e reconstrói os trechos a partir dos predecessores.
std::vector<Trecho> dijkstra(const Grafo& g, int origem, int destino, Criterio c) {
    std::vector<double> melhor(g.tamanho(), std::numeric_limits<double>::infinity());
    std::vector<int> pai(g.tamanho(), -1);
    using Entrada = std::pair<double,int>;
    std::priority_queue<Entrada,std::vector<Entrada>,std::greater<Entrada>> fila;
    melhor.at(origem) = 0;
    fila.push({0,origem});
    while (!fila.empty()) {
        auto [total,u] = fila.top();
        fila.pop();
        if (total > melhor[u]) continue;
        if (u == destino) break;
        for (const auto& a : g.vizinhos(u)) {
            double candidato = total + valor(a.pesos,c);
            if (candidato >= melhor[a.destino]) continue;
            melhor[a.destino] = candidato;
            pai[a.destino] = u;
            fila.push({candidato,a.destino});
        }
    }
    std::vector<Trecho> rota;
    for (int v = destino; pai.at(v) != -1; v = pai[v]) rota.push_back({pai[v],v});
    std::reverse(rota.begin(),rota.end());
    return rota;
}

// Liga uma cidade nova por vez pela aresta mais leve que sai da árvore.
std::vector<Trecho> prim(const Grafo& g, Criterio c) {
    std::vector<Trecho> arvore;
    if (!g.tamanho()) return arvore;
    const double infinito = std::numeric_limits<double>::infinity();
    std::vector<double> melhor(g.tamanho(), infinito);
    std::vector<int> pai(g.tamanho(), -1);
    std::vector<bool> incluido(g.tamanho(), false);
    melhor[0] = 0;
    for (int passo = 0; passo < g.tamanho(); ++passo) {
        int u = -1;
        for (int v = 0; v < g.tamanho(); ++v)
            if (!incluido[v] && (u == -1 || melhor[v] < melhor[u])) u = v;
        if (melhor[u] == infinito) throw std::runtime_error("Grafo desconexo");
        incluido[u] = true;
        if (pai[u] != -1) arvore.push_back({pai[u],u});
        for (const auto& a : g.vizinhos(u)) {
            double peso = valor(a.pesos,c);
            if (incluido[a.destino] || peso >= melhor[a.destino]) continue;
            melhor[a.destino] = peso;
            pai[a.destino] = u;
        }
    }
    return arvore;
}

// Soma custo, distância e duração dos trechos de uma rota ou árvore.
Pesos somar(const Grafo& g, const std::vector<Trecho>& trechos) {
    Pesos total{};
    for (Trecho t : trechos) {
        const Pesos* p = g.obterPeso(t.origem,t.destino);
        if (!p) throw std::invalid_argument("Trecho inexistente");
        total.km += p->km;
        total.horas += p->horas;
        total.euros += p->euros;
    }
    return total;
}
