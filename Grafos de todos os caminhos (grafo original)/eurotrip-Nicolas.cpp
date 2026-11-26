#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// Agrupa as três medidas de um trecho na mesma ordem usada em pares[].
struct Pesos {
    double km, horas, euros;
};

// Representa uma saída da cidade cuja lista contém esta aresta.
struct Aresta {
    int destino;
    Pesos pesos;
};

// Identifica qual medida será minimizada em uma consulta.
enum class Criterio { Custo, Distancia, Tempo };

// Seleciona o valor de um trecho que Dijkstra vai somar ou que a matriz vai exibir.
double peso(Pesos p, Criterio criterio) {
    if (criterio == Criterio::Custo) return p.euros;
    if (criterio == Criterio::Distancia) return p.km;
    return p.horas;
}

// Lista de adjacencia das aulas 11 e 13, com tres pesos por vizinho.
class GrafoListaPonderado {
    // O índice identifica a mesma cidade nos dois vetores.
    vector<string> cidades;
    vector<list<Aresta>> listaAdj;

public:
    // Conta as cidades cadastradas para dimensionar as buscas e percorrer o grafo.
    int numVertices() const { return static_cast<int>(cidades.size()); }
    // Traduz o índice de um vértice para o nome que aparece no terminal.
    const string& cidade(int v) const { return cidades.at(v); }
    // Devolve a lista de saídas por referência para percorrer os trechos sem copiar a lista.
    const list<Aresta>& obterVizinhos(int v) const { return listaAdj.at(v); }

    // Cadastra uma cidade com lista de saídas vazia e devolve seu índice para criar ligações.
    int adicionarVertice(const string& nome) {
        if (find(cidades.begin(), cidades.end(), nome) != cidades.end())
            throw invalid_argument("Cidade repetida: " + nome);
        cidades.push_back(nome);
        listaAdj.emplace_back();
        return numVertices() - 1;
    }

    // Valida os extremos e os pesos e insere um trecho dirigido na lista da origem.
    void adicionarAresta(int origem, int destino, Pesos p) {
        if (origem < 0 || origem >= numVertices() || destino < 0 ||
            destino >= numVertices() || origem == destino)
            throw invalid_argument("Extremos invalidos para o trecho.");
        // As três medidas precisam ser finitas e não negativas, pois qualquer uma pode orientar a busca.
        for (double valor : {p.km, p.horas, p.euros})
            if (!isfinite(valor) || valor < 0)
                throw invalid_argument("Peso invalido para Dijkstra.");
        for (const Aresta& a : obterVizinhos(origem))
            if (a.destino == destino) throw invalid_argument("Trecho repetido.");
        listaAdj[origem].push_back({destino, p});
    }
};

// Os seis primeiros índices preservam a numeração das cidades europeias no menu.
enum Cidade { Roma, Paris, Oslo, Berna, Budapeste, Bratislava, Brasil };

// Monta o grafo com os trechos documentados nos slides e a origem exigida pelo enunciado.
GrafoListaPonderado carregarEurotrip() {
    GrafoListaPonderado g;
    // A ordem do cadastro abaixo faz os nomes do enum coincidirem com os índices do grafo.
    for (const string nome : {"Roma", "Paris", "Oslo", "Berna", "Budapeste", "Bratislava", "Brasil"})
        g.adicionarVertice(nome);

    struct Ligacao { int origem, destino; Pesos pesos; };
    const Ligacao pares[] = {
        {Roma, Paris,           {1415, 17, 311}},
        {Roma, Oslo,            {2689, 25, 758}},
        {Roma, Berna,           {886,   9, 207}},
        {Roma, Budapeste,       {1162, 12, 208}},
        {Roma, Bratislava,      {1114, 11, 246}},
        {Paris, Oslo,           {1941, 18, 549}},
        {Paris, Berna,          {528,   6, 124}},
        {Paris, Budapeste,      {1485, 13, 289}},
        {Paris, Bratislava,     {1290, 12, 290}},
        {Oslo, Berna,           {1866, 23, 411}},
        {Oslo, Budapeste,       {2179, 21, 617}},
        {Oslo, Bratislava,      {1755, 21, 386}},
        {Berna, Budapeste,      {1060, 10, 248}},
        {Berna, Bratislava,     {883,   9, 205}},
        {Budapeste, Bratislava, {201,   2,  40}},
        // Partida de São Paulo (GRU). Estimativas de ida por pessoa, em km, horas e EUR.
        {Brasil, Roma,          {9434, 11.50, 337}},
        {Brasil, Paris,         {9422, 15.42, 344}},
        {Brasil, Berna,         {9811, 17.73, 394}} 
    };
    // O modelo usa os mesmos pesos na volta, inclusive nos três trechos internacionais.
    for (const Ligacao& par : pares) {
        g.adicionarAresta(par.origem, par.destino, par.pesos);
        g.adicionarAresta(par.destino, par.origem, par.pesos);
    }
    return g;
}

// Faz uma busca em largura a partir de cada cidade para verificar o alcance nos dois sentidos.
bool fortementeConexo(const GrafoListaPonderado& g) {
    // Em um grafo dirigido, cada cidade precisa alcancar todas as outras.
    for (int origem = 0; origem < g.numVertices(); ++origem) {
        vector<bool> visitado(g.numVertices(), false);
        queue<int> fila;
        visitado[origem] = true;
        fila.push(origem);
        while (!fila.empty()) {
            int atual = fila.front();
            fila.pop();
            for (const Aresta& a : g.obterVizinhos(atual)) {
                if (visitado[a.destino]) continue;
                // Marca ao enfileirar para que cada cidade entre só uma vez nesta busca.
                visitado[a.destino] = true;
                fila.push(a.destino);
            }
        }
        // Um único destino fora do alcance desta origem já invalida a conectividade forte.
        if (find(visitado.begin(), visitado.end(), false) != visitado.end()) return false;
    }
    return g.numVertices() > 0;
}

// Devolve a sequência de cidades que minimiza o critério escolhido entre origem e destino.
vector<int> dijkstra(const GrafoListaPonderado& g, int origem, int destino, Criterio criterio) {
    const double infinito = numeric_limits<double>::infinity();
    // Começamos sem caminho conhecido. O valor acumulado pode representar euros, km ou horas.
    vector<double> distancias(g.numVertices(), infinito);
    // Cada posição guarda o predecessor no melhor caminho conhecido. -1 marca o início.
    vector<int> anterior(g.numVertices(), -1);
    vector<bool> visitado(g.numVertices(), false);
    using Entrada = pair<double, int>; // (peso acumulado, vertice)
    // Com greater, o topo é o par de menor peso acumulado.
    priority_queue<Entrada, vector<Entrada>, greater<Entrada>> fila;
    distancias.at(origem) = 0;
    fila.push({0, origem});

    while (!fila.empty()) {
        int u = fila.top().second;
        fila.pop();
        // Uma melhora pode gerar outra entrada para u. Só a primeira retirada precisa ser expandida.
        if (visitado[u]) continue;
        visitado[u] = true;
        // Com pesos não negativos, o destino já tem valor mínimo quando sai da fila pela primeira vez.
        if (u == destino) break;
        for (const Aresta& a : g.obterVizinhos(u)) {
            // Relaxamento: o caminho passando por u melhora o valor conhecido?
            double novo = distancias[u] + peso(a.pesos, criterio);
            if (novo < distancias[a.destino]) {
                distancias[a.destino] = novo;
                anterior[a.destino] = u;
                fila.push({novo, a.destino});
            }
        }
    }
    vector<int> caminho;
    if (!visitado.at(destino)) return caminho;
    // Segue os predecessores de trás para frente. A inversão coloca a origem no início da rota.
    for (int v = destino; v != -1; v = anterior[v]) caminho.push_back(v);
    reverse(caminho.begin(), caminho.end());
    return caminho;
}

// Mostra a sequência encontrada e soma euros, quilômetros e horas dos trechos percorridos.
void exibirRota(const GrafoListaPonderado& g, const vector<int>& caminho) {
    if (caminho.empty()) {
        cout << "Destino inacessivel.\n";
        return;
    }
    Pesos total = {0, 0, 0};
    cout << g.cidade(caminho.front());
    for (size_t i = 1; i < caminho.size(); ++i) {
        cout << " -> " << g.cidade(caminho[i]);
        // Procura a ligação entre duas cidades consecutivas antes de somar os três pesos.
        for (const Aresta& a : g.obterVizinhos(caminho[i - 1])) {
            if (a.destino == caminho[i]) {
                total.km += a.pesos.km;
                total.horas += a.pesos.horas;
                total.euros += a.pesos.euros;
                break;
            }
        }
    }
    cout << "\n" << total.euros << " EUR | " << total.km << " km | " << total.horas << " h\n";
}

// Exibe os pesos dos trechos diretos para comparar o grafo cadastrado com a matriz dos slides.
void exibirMatriz(const GrafoListaPonderado& g, Criterio criterio) {
    cout << setw(12) << "De / Para";
    for (int v = 0; v < g.numVertices(); ++v) cout << setw(12) << g.cidade(v);
    cout << '\n';
    for (int u = 0; u < g.numVertices(); ++u) {
        vector<double> linha(g.numVertices(), numeric_limits<double>::infinity());
        // A diagonal vale zero. As demais colunas só recebem valor se houver uma saída correspondente.
        linha[u] = 0;
        for (const Aresta& a : g.obterVizinhos(u)) linha[a.destino] = peso(a.pesos, criterio);
        cout << setw(12) << g.cidade(u);
        for (double valor : linha) {
            if (isinf(valor)) cout << setw(12) << "INF";
            else cout << setw(12) << valor;
        }
        cout << '\n';
    }
}

// Aceita o numero completo da opcao; 0 encerra tambem em caso de fim da entrada.
// Lê e valida uma escolha do menu para que o programa receba um inteiro dentro da faixa.
int lerOpcao(const string& mensagem, int maximo) {
    string linha;
    while (cout << mensagem && getline(cin, linha)) {
        istringstream entrada(linha);
        int numero;
        // A leitura de um caractere extra detecta entradas como 2abc ou dois números na mesma linha.
        char sobra;
        if (entrada >> numero && !(entrada >> sobra) && numero >= 0 && numero <= maximo)
            return numero;
        cout << "Digite um numero de 0 a " << maximo << ".\n";
    }
    return 0;
}

// Carrega e valida o grafo, depois coordena as consultas de rotas e a exibição das matrizes.
int main() {
    GrafoListaPonderado g = carregarEurotrip();
    int arcos = 0;
    for (int u = 0; u < g.numVertices(); ++u) {
        const auto& vizinhos = g.obterVizinhos(u);
        if (vizinhos.size() < 3) {
            cerr << "Menos de tres saidas em " << g.cidade(u) << ".\n";
            return 1;
        }
        arcos += static_cast<int>(vizinhos.size());
    }
    if (g.numVertices() != 7 || arcos != 36 || !fortementeConexo(g)) {
        cerr << "Confira a quantidade de cidades, os arcos e a conectividade.\n";
        return 1;
    }
    // Duas casas decimais afetam a impressão. Dijkstra usa os pesos com a precisão cadastrada.
    cout << fixed << setprecision(2);
    cout << "Origem: Brasil - Sao Paulo (GRU)\n";
    cout << "Eurotrip | " << g.numVertices() << " vertices | " << arcos
         << " arcos cadastrados\n";
    while (true) {
        int opcao = lerOpcao("\n1 - Selecionar destino\n2 - Ver matrizes\n0 - Sair\nOpcao: ", 2);
        if (opcao == 0) break;
        if (opcao == 2) {
            cout << "\nCusto (EUR)\n";
            exibirMatriz(g, Criterio::Custo);
            cout << "\nDistancia (km)\n";
            exibirMatriz(g, Criterio::Distancia);
            cout << "\nTempo (horas decimais)\n";
            exibirMatriz(g, Criterio::Tempo);
            continue;
        }
        for (int v = Roma; v <= Bratislava; ++v)
            cout << v + 1 << " - " << g.cidade(v) << '\n';
        int destino = lerOpcao("Destino (0 para sair): ", Bratislava + 1);
        if (destino == 0) break;
        cout << "\nMenor custo: ";
        exibirRota(g, dijkstra(g, Brasil, destino - 1, Criterio::Custo));
        cout << "\nMenor distancia: ";
        exibirRota(g, dijkstra(g, Brasil, destino - 1, Criterio::Distancia));
        cout << "\nMenor tempo: ";
        exibirRota(g, dijkstra(g, Brasil, destino - 1, Criterio::Tempo));
    }
}
