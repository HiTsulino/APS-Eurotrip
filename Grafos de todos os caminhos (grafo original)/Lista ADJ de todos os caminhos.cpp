#include <iostream>
#include <vector>
#include <list>

using namespace std;

class GrafoLista{
private:
	int numVertices;
	vector<list<int>> listaAdj;
public:
	GrafoLista(int vertices){
		numVertices = vertices;
		listaAdj.resize(vertices);
	}
	
	bool ehValido(int origem, int destino){
		return origem >= 0 && origem < numVertices && destino >= 0 && destino < numVertices;
	}
	
	void adicionarAresta(int origem, int destino){
		if(ehValido(origem,destino)){
			listaAdj[origem].push_back(destino);//onde quero colocar no final
			listaAdj[destino].push_back(origem);
		}
	}
	
	void imprimir(){
		for(int i = 0; i< numVertices; i++){
			cout << i << ": ";
			for(int vizinho : listaAdj[i]){
				cout << vizinho << " ";
			}
			cout << endl;
		}cout << endl << endl;
	}
	
	void remover(int origem, int destino){
		if(ehValido(origem,destino)){
			listaAdj[origem].remove(destino);//onde quero colocar no final
			listaAdj[destino].remove(origem);
		}
	}
	
	bool existeAresta(int origem, int destino){
		if(ehValido(origem,destino)){
			for(int vizinho : listaAdj[origem]){
				if(vizinho == destino){
					return true;
				}
			}
		}
	}
};
	


int main(){
	GrafoLista grafo(6);
	
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
	
	grafo.imprimir();

	return 0;
}