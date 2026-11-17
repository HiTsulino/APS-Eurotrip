
#include <iostream>
#include <vector>

using namespace std;

class GrafoMatriz {
private:
    int numVertice;
    vector<vector<int>> matriz;

public:
    // Construtor
    GrafoMatriz(int vertices) : numVertice(vertices) {
        matriz.resize(vertices, vector<int>(vertices, 0));//inicia os vertices com 0
    }

    // Adicionar aresta
    void adicionarAresta(int origem, int destino) {
        if (origem >= 0 && origem < numVertice &&
            destino >= 0 && destino < numVertice) {

            matriz[origem][destino] = 1;

            // Grafo nao direcionado
            matriz[destino][origem] = 1;
        }
    }

    // Remover aresta
    void removerAresta(int origem, int destino) {
        if (origem >= 0 && origem < numVertice &&
            destino >= 0 && destino < numVertice) {

            matriz[origem][destino] = 0;
            matriz[destino][origem] = 0;
        }
    }
	
    // Verificar se existe aresta
    bool existeAresta(int origem, int destino) {
        if (origem >= 0 && origem < numVertice &&
            destino >= 0 && destino < numVertice) {

            return matriz[origem][destino] == 1;
        }

        return false;
    }

    // Imprimir matriz de adjacencia
    void imprimir() {
        cout << "  | ";

        for (int i = 0; i < numVertice; i++) {
            cout << i << " ";
        }

        cout << endl << "-----------" << endl;

        for (int i = 0; i < numVertice; i++) {
            cout << i << " | ";

            for (int j = 0; j < numVertice; j++) {
                cout << matriz[i][j] << " ";
            }

            cout << endl;
        }

        cout << endl;
    }
};

int main() {

    GrafoMatriz grafo(6);

    // Adicionar arestas
    grafo.adicionarAresta(0, 1);
	grafo.adicionarAresta(0, 2);
	grafo.adicionarAresta(0, 3);
	grafo.adicionarAresta(0, 4);
	grafo.adicionarAresta(0, 5);

	grafo.adicionarAresta(1, 2);
	grafo.adicionarAresta(1, 3);
	grafo.adicionarAresta(1, 4);
	grafo.adicionarAresta(1, 5);

	grafo.adicionarAresta(2, 3);
	grafo.adicionarAresta(2, 4);
	grafo.adicionarAresta(2, 5);

	grafo.adicionarAresta(3, 4);
	grafo.adicionarAresta(3, 5);

	grafo.adicionarAresta(4, 5);

    // Imprimir matriz
    cout << "Matriz de Adjacencia:" << endl;
    grafo.imprimir();

    return 0;
}
