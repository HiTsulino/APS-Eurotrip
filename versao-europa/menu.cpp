#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Menu de Nicolas, visualizações de Igor e comparação das árvores de Viviane.

namespace {
const std::pair<Criterio,const char*> criterios[] = {
    {Criterio::Custo,"Custo (EUR)"}, {Criterio::Distancia,"Distancia (km)"},
    {Criterio::Tempo,"Tempo (h)"}
};

// Lê um número dentro da faixa e trata fim de entrada como saída.
int opcao(const char* pergunta, int maximo) {
    std::string linha;
    while (std::cout << pergunta && std::getline(std::cin,linha)) {
        std::istringstream entrada(linha);
        int n;
        char sobra;
        if (entrada >> n && !(entrada >> sobra) && n >= 0 && n <= maximo) return n;
        std::cout << "Escolha de 0 a " << maximo << ".\n";
    }
    return 0;
}

// Exibe a lista de vizinhos, a matriz de presença e as três matrizes ponderadas.
void mostrarGrafo(const Grafo& g) {
    for (int u = 0; u < g.tamanho(); ++u) {
        std::cout << g.cidade(u) << ":";
        for (auto a : g.vizinhos(u)) std::cout << "  " << g.cidade(a.destino);
        std::cout << '\n';
    }
    for (int matriz = -1; matriz < 3; ++matriz) {
        std::cout << "\n" << (matriz < 0 ? "Presenca" : criterios[matriz].second) << "\n"
                  << std::setw(12) << "De / Para";
        for (int v = 0; v < g.tamanho(); ++v) std::cout << std::setw(13) << g.cidade(v);
        std::cout << '\n';
        for (int u = 0; u < g.tamanho(); ++u) {
            std::cout << std::setw(12) << g.cidade(u);
            for (int v = 0; v < g.tamanho(); ++v) {
                const Pesos* p = g.obterPeso(u,v);
                std::cout << std::setw(13);
                if (matriz < 0) std::cout << (p != nullptr ? 1 : 0);
                else if (p) std::cout << valor(*p,criterios[matriz].first);
                else std::cout << (u == v ? "0" : "-");
            }
            std::cout << '\n';
        }
    }
}

// Imprime a duração arredondada ao minuto somente depois de somar todos os trechos.
void mostrarTotais(Pesos p) {
    auto minutos = std::llround(p.horas * 60);
    std::cout << p.euros << " EUR | " << p.km << " km | " << minutos/60 << "h"
              << std::setfill('0') << std::setw(2) << minutos%60 << std::setfill(' ') << '\n';
}

// Seleciona os extremos permitidos e mostra os três caminhos mínimos.
void mostrarRotas(const Grafo& g, Versao versao) {
    constexpr int capitais = 6;
    for (int v = 0; v < capitais; ++v) std::cout << v+1 << " - " << g.cidade(v) << '\n';
    int origem = g.tamanho()-1;
    if (versao == Versao::Europa) {
        origem = opcao("Origem (0 volta): ",capitais)-1;
        if (origem < 0) return;
    }
    int destino;
    do {
        destino = opcao("Destino (0 volta): ",capitais)-1;
        if (destino < 0) return;
        if (destino == origem) std::cout << "Escolha outra cidade.\n";
    } while (destino == origem);
    for (auto [criterio,nome] : criterios) {
        auto rota = dijkstra(g,origem,destino,criterio);
        std::cout << nome << ": " << g.cidade(origem);
        for (Trecho t : rota) std::cout << " -> " << g.cidade(t.destino);
        std::cout << '\n';
        mostrarTotais(somar(g,rota));
    }
}

// Lista as conexões de cada árvore e compara os totais na mesma ordem de unidades.
void mostrarArvores(const Grafo& g) {
    std::vector<Pesos> totais;
    for (auto [criterio,nome] : criterios) {
        auto arvore = prim(g,criterio);
        std::cout << "\nAGM por " << nome << '\n';
        for (Trecho t : arvore) {
            std::cout << g.cidade(t.origem) << " - " << g.cidade(t.destino) << " | ";
            mostrarTotais(*g.obterPeso(t.origem,t.destino));
        }
        totais.push_back(somar(g,arvore));
    }
    std::cout << "\nComparacao das arvores\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << criterios[i].second << ": ";
        mostrarTotais(totais[i]);
    }
}
}

// Encontra e compara as melhores rotas para cada criterio.
void mostrarRotasOtimizadas(const Grafo& g) {

    std::cout << "\nCAPITAIS DISPONIVEIS\n";

    for (int v = 0; v < g.tamanho(); ++v) {
        std::cout << v + 1 << " - " << g.cidade(v) << '\n';
    }

    int origem = opcao("\nEscolha a cidade de origem (0 volta): ",
                       g.tamanho()) - 1;

    if (origem < 0) return;

    std::vector<Pesos> totais;

    std::cout << "\n===== ROTAS OTIMIZADAS =====\n";

    for (auto [criterio, nome] : criterios) {

        auto rota = rotaOtimizada(g, origem, criterio);

        std::cout << "\nRota por " << nome << ":\n";

        std::cout << g.cidade(origem);

        for (Trecho t : rota) {
            std::cout << " -> " << g.cidade(t.destino);
        }

        std::cout << "\nTrechos da viagem:\n";

        for (Trecho t : rota) {

            const Pesos* p = g.obterPeso(t.origem, t.destino);

            std::cout << g.cidade(t.origem)
                      << " - " << g.cidade(t.destino) << " | ";

            mostrarTotais(*p);
        }

        Pesos total = somar(g, rota);

        totais.push_back(total);

        std::cout << "Total da rota: ";
        mostrarTotais(total);
    }

    std::cout << "\n===== COMPARACAO DAS ROTAS =====\n";

    for (int i = 0; i < 3; ++i) {

        std::cout << criterios[i].second << ": ";

        mostrarTotais(totais[i]);
    }
}



// Valida o cadastro da versão selecionada e atende as consultas até a saída.
int executar(Versao versao) {
    Grafo g = carregar(versao);
    int arcos = 0;

    for (int u = 0; u < g.tamanho(); ++u) {
        if (g.vizinhos(u).size() < 3) {
            std::cerr << "Poucas ligacoes em "
                      << g.cidade(u) << '\n';
            return 1;
        }

        arcos += static_cast<int>(g.vizinhos(u).size());
    }

    bool europa = versao == Versao::Europa;

    if (!conexo(g) ||
        g.tamanho() != (europa ? 6 : 7) ||
        arcos != (europa ? 30 : 36)) {

        std::cerr << "Confira o cadastro e a conectividade do grafo.\n";
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2)
              << (europa
                  ? "A viagem parte do Brasil. Os cálculos começam na cidade europeia escolhida."
                  : "Origem das rotas: São Paulo (GRU).")
              << '\n';

    while (true) {

        int escolha = opcao(
            "\n1 - Caminhos minimos"
            "\n2 - Grafo e matrizes"
            "\n3 - AGM e comparacao"
            "\n4 - Rotas Otimizadas e comparacao"
            "\n0 - Sair"
            "\nOpcao: ", 4
        );

        if (!escolha) return 0;

        if (escolha == 1) mostrarRotas(g, versao);
        if (escolha == 2) mostrarGrafo(g);
        if (escolha == 3) mostrarArvores(g);
        if (escolha == 4) mostrarRotasOtimizadas(g);
    }
}
