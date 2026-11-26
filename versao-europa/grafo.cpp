#include <algorithm>
#include <cmath>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

// Tipos usados no cadastro, nas rotas e na AGM.
enum class Versao { Europa, SaoPaulo };
enum class Criterio { Custo, Distancia, Tempo };
struct Pesos { double km, horas, euros; };
struct Aresta { int destino; Pesos pesos; };
struct Trecho { int origem, destino; };

class Grafo {
    std::vector<std::string> nomes;
    std::vector<std::vector<Aresta>> adj;
public:
    int adicionarVertice(const std::string& nome);
    void adicionarAresta(int u, int v, Pesos pesos);
    void removerAresta(int u, int v);
    const Pesos* obterPeso(int u, int v) const;
    bool existeAresta(int u, int v) const;
    // Informa quantas cidades estão cadastradas.
    int tamanho() const { return static_cast<int>(nomes.size()); }
    // Traduz o índice da cidade para o nome exibido.
    const std::string& cidade(int v) const { return nomes.at(v); }
    // Percorre as ligações de uma cidade sem copiar seus dados.
    const std::vector<Aresta>& vizinhos(int v) const { return adj.at(v); }
};

// Cadastro de Igor e lista ponderada de Nicolas, com ligações nos dois sentidos.

// Cadastra uma cidade e devolve o índice que será usado nas ligações.
int Grafo::adicionarVertice(const std::string& nome) {
    if (std::find(nomes.begin(), nomes.end(), nome) != nomes.end())
        throw std::invalid_argument("Cidade repetida");
    nomes.push_back(nome);
    adj.emplace_back();
    return tamanho() - 1;
}

// Localiza os três pesos de um trecho direto, se ele existir.
const Pesos* Grafo::obterPeso(int u, int v) const {
    if (u < 0 || u >= tamanho()) return nullptr;
    for (const auto& a : adj[u])
        if (a.destino == v) return &a.pesos;
    return nullptr;
}

// Verifica se duas cidades têm ligação direta.
bool Grafo::existeAresta(int u, int v) const { return obterPeso(u, v) != nullptr; }

// Valida os extremos e os pesos antes de cadastrar ida e volta.
void Grafo::adicionarAresta(int u, int v, Pesos p) {
    if (u < 0 || v < 0 || u >= tamanho() || v >= tamanho() || u == v || existeAresta(u,v))
        throw std::invalid_argument("Ligacao invalida ou repetida");
    for (double n : {p.km, p.horas, p.euros})
        if (!std::isfinite(n) || n < 0) throw std::invalid_argument("Peso invalido");
    adj[u].push_back({v,p});
    adj[v].push_back({u,p});
}

// Remove as duas direções da ligação quando ela existe.
void Grafo::removerAresta(int u, int v) {
    if (!existeAresta(u,v)) return;
    for (Trecho t : {Trecho{u,v}, Trecho{v,u}}) {
        auto& lista = adj[t.origem];
        for (auto it = lista.begin(); it != lista.end(); ++it)
            if (it->destino == t.destino) { lista.erase(it); break; }
    }
}

// Carrega os pares da planilha e, na versão brasileira, os três trechos de GRU.
Grafo carregar(Versao versao) {
    Grafo g;
    for (auto nome : {"Roma","Paris","Oslo","Berna","Budapeste","Bratislava"})
        g.adicionarVertice(nome);
    struct Par { int u, v; Pesos pesos; };
    const Par europa[] = {
        {0,1,{1414.8,17.25,311}}, {0,2,{2688.6,25+52/60.0,758}},
        {0,3,{886,9+5/60.0,207}}, {0,4,{1162.1,11+50/60.0,208}},
        {0,5,{1114,11+23/60.0,246}}, {1,2,{1941,18+28/60.0,549}},
        {1,3,{528,6+20/60.0,124}}, {1,4,{1485,13+29/60.0,289}},
        {1,5,{1290.2,12+11/60.0,290}}, {2,3,{1866,22.76,411}},
        {2,4,{2179,21.15,617}}, {2,5,{1754.9,21.4,386}},
        {3,4,{1060,10+23/60.0,248}}, {3,5,{882.8,9+1/60.0,205}},
        {4,5,{201,1+52/60.0,40}}
    };
    for (const auto& par : europa) g.adicionarAresta(par.u, par.v, par.pesos);
    if (versao == Versao::SaoPaulo) {
        int gru = g.adicionarVertice("São Paulo");
        g.adicionarAresta(gru,0,{9434,11.50,337});
        g.adicionarAresta(gru,1,{9422,15.42,344});
        g.adicionarAresta(gru,3,{9811,17.73,394});
    }
    return g;
}

// Verifica por busca em largura se todas as cidades da rede simétrica se alcançam.
bool conexo(const Grafo& g) {
    if (!g.tamanho()) return false;
    std::vector<bool> visto(g.tamanho(), false);
    std::queue<int> fila;
    fila.push(0);
    visto[0] = true;
    while (!fila.empty()) {
        int u = fila.front();
        fila.pop();
        for (const auto& a : g.vizinhos(u)) {
            if (visto[a.destino]) continue;
            visto[a.destino] = true;
            fila.push(a.destino);
        }
    }
    return std::find(visto.begin(), visto.end(), false) == visto.end();
}
