#include <iostream>
#include <vector>
#include <list>
#include <climits>
#include <utility>
#include <iomanip>

using namespace std;

class GrafoListaPonderado {
public:
    int numVertices;
    vector<list<pair<int, int>>> listaAdj;

    GrafoListaPonderado(int vertices) : numVertices(vertices) {
        listaAdj.resize(vertices);
    }

    void adicionarAresta(int origem, int destino, int peso) {
        if (origem >= 0 && origem < numVertices &&
            destino >= 0 && destino < numVertices) {

            listaAdj[origem].push_back({destino, peso});

            // Se o grafo for NÃO direcionado:
            // listaAdj[destino].push_back({origem, peso});
        }
        else {
            cout << "Erro: vertices invalidos" << endl;
        }
    }

    const list<pair<int, int>>& obterVizinhos(int vertice) const {
        static const list<pair<int, int>> vazio;

        if (vertice >= 0 && vertice < numVertices) {
            return listaAdj[vertice];
        }

        return vazio;
    }

    void imprimir() const {
        for (int i = 0; i < numVertices; i++) {

            cout << "V" << i << ": ";

            for (const auto& vizinho : listaAdj[i]) {
                cout << "("
                     << vizinho.first
                     << ", peso=" << vizinho.second
                     << ") ";
            }

            cout << endl;
        }
    }
};

class GrafoMatrizPonderado {
public:
    int numVertices;
    vector<vector<int>> matrizAdj;

    GrafoMatrizPonderado(int vertices)
        : matrizAdj(vertices, vector<int>(vertices, INT_MAX)) {

        numVertices = vertices;

        for (int i = 0; i < vertices; i++) {
            matrizAdj[i][i] = 0;
        }
    }

    void adicionarAresta(int origem, int destino, int peso) {

        if (origem >= 0 && origem < numVertices &&
            destino >= 0 && destino < numVertices) {

            matrizAdj[origem][destino] = peso;
			matrizAdj[destino][origem] = peso;

            // Se o grafo for NÃO direcionado:
            // matrizAdj[destino][origem] = peso;
        }
        else {
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

    int obterNumVertices() const {
        return numVertices;
    }

    void imprimir() const {

        cout << "    ";

        for (int i = 0; i < numVertices; i++) {
            cout << setw(4) << "V" << i;
        }

        cout << endl;

        for (int i = 0; i < numVertices; i++) {

            cout << "V" << i << " |";

            for (int j = 0; j < numVertices; j++) {

    			if (matrizAdj[i][j] == INT_MAX)
        			cout << setw(5) << "N";
    			else
      				cout << setw(5) << matrizAdj[i][j];
			}

            cout << endl;
        }
    }
};

int main() {
	cout << "Grafo Matriz Ponderada do tempo entre as Capitais em Horas" << endl;
    GrafoMatrizPonderado grafo(6);

    // Roma
    grafo.adicionarAresta(0, 1, 17);
    grafo.adicionarAresta(0, 2, 25);
    grafo.adicionarAresta(0, 3, 9);
    grafo.adicionarAresta(0, 4, 12);
    grafo.adicionarAresta(0, 5, 11);

    // Paris
    grafo.adicionarAresta(1, 2, 18);
    grafo.adicionarAresta(1, 3, 6);
    grafo.adicionarAresta(1, 4, 13);
    grafo.adicionarAresta(1, 5, 12);

    // Oslo
    grafo.adicionarAresta(2, 3, 23);
    grafo.adicionarAresta(2, 4, 21);
    grafo.adicionarAresta(2, 5, 21);

    // Berna
    grafo.adicionarAresta(3, 4, 10);
    grafo.adicionarAresta(3, 5, 9);

    // Budapeste
    grafo.adicionarAresta(4, 5, 2);

    grafo.imprimir();

    return 0;
}

