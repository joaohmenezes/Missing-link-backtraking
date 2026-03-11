/*
Trabalho Avaliativo de Projeto e Análise de Algoritmos (ECOI2207)
Prof. Giovani Bernardes Vitor

-> build: g++ -fdiagnostics-color=always -g "main.cpp" "application.cpp" "tinyxml2.cpp" -o "main.exe"

Frederico Pires de Moraes Gomes - 2023006205
Gustavo Felipe Ferreira Soares - 2023002162
João Henquique de Menezes Pereira Santos - 2023002313
*/



#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "tinyxml2.h"

using namespace std;

class EloMaluco {
private:
    vector<vector<string>> estado; // Matriz representando o estado atual
    int tempo; // Para contagem de movimentos
    vector<string> acoes;
    vector<vector<vector<string>>> estadosVisitados;
    int profundidade = 0;

    int calcularHeuristica();

public:
    // Construtor
    EloMaluco();

    // Método para carregar estado inicial a partir de um arquivo XML
    bool carregarEstado(const string &arquivo);

    // Método para gerar estado inicial aleatório
    void gerarEstadoAleatorio();

    // Método para exibir o estado atual
    void exibirEstado() const;

    // Método para verificar se o estado atual é solução
    bool ehSolucao() const;

    // Métodos para realizar ações no jogo
    void rotacionarSuperiorDireita();
    void rotacionarSuperiorEsquerda();
    void rotacionarInferiorDireita();
    void rotacionarInferiorEsquerda();
    void moverFaceAbaixoParaCima();
    void moverFaceAcimaParaBaixo();

    int getTempoMovimentos() const;    
    void resetTempo();
    const vector<string>& getAcoes() const { return acoes; }

    bool estadoJaVisitado() const;
    void adicionarEstadoVisitado();
    void limparEstadosVisitados();
    void salvarSolucao(const string& arquivo);
    bool resolverJogo();
};

#endif // __APPLICATION_HPP__
