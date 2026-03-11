/*
Trabalho Avaliativo de Projeto e Análise de Algoritmos (ECOI2207)

-> build: g++ -fdiagnostics-color=always -g "main.cpp" "application.cpp" "tinyxml2.cpp" -o "main.exe"


João Henquique de Menezes Pereira Santos - 
*/

#include <iostream>
#include "application.hpp"

using namespace std;

int main() {
    class EloMaluco jogo; 
    
    cout << "Elo Maluco - Solucionador" << endl;
    cout << "=========================" << endl;
    
    // Menu de escolha do estado inicial
    cout << "\n1 - Carregar estado de arquivo XML\n2 - Gerar estado aleatório\nEscolha: ";
    int escolha;
    cin >> escolha;

    // Inicialização do estado
    if (escolha == 1) {
        cout << "\nNome do arquivo XML: ";
        string arquivo;
        cin >> arquivo;
        
        if (!jogo.carregarEstado(arquivo)) {
            cerr << "Erro ao carregar arquivo." << endl;
            return 1;
        }
    } else {
        jogo.gerarEstadoAleatorio();
    }

    cout << "\nEstado Inicial:" << endl;
    jogo.exibirEstado();

    // Tenta resolver o puzzle
    cout << "\nResolvendo..." << endl;
    if (jogo.resolverJogo()) {
        cout << "\nSolução encontrada!" << endl;
        cout << "Número de movimentos: " << jogo.getTempoMovimentos() << endl;
        
        cout << "\nEstado Final:" << endl;
        jogo.exibirEstado();
        cout << "\nSolução salva em 'EloMaluco_solucao.xml'" << endl;
    } else {
        cout << "\nNão foi possível encontrar uma solução." << endl;
    }

    return 0;
}