/*
Trabalho Avaliativo de Projeto e Análise de Algoritmos (ECOI2207)
Prof. Giovani Bernardes Vitor

-> build: g++ -fdiagnostics-color=always -g "main.cpp" "application.cpp" "tinyxml2.cpp" -o "main.exe"

Frederico Pires de Moraes Gomes - 2023006205
Gustavo Felipe Ferreira Soares - 2023002162
João Henquique de Menezes Pereira Santos - 2023002313
*/



#include "application.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include "tinyxml2.h"

using namespace std;

// Construtor
EloMaluco::EloMaluco()
{
    estado = vector<vector<string>>(4, vector<string>(4, ""));
    tempo = 0;
    estadosVisitados.clear();
}

// Método para carregar estado inicial a partir de um arquivo XML
bool EloMaluco::carregarEstado(const string &arquivo)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(arquivo.c_str());

    if (result != tinyxml2::XML_SUCCESS)
    {
        cerr << "Erro ao carregar arquivo XML: " << arquivo << endl;
        return false;
    }

    tinyxml2::XMLElement *root = doc.FirstChildElement("EloMaluco");
    if (!root)
    {
        cerr << "Arquivo XML inválido: elemento raiz não encontrado" << endl;
        return false;
    }

    tinyxml2::XMLElement *estadoAtual = root->FirstChildElement("EstadoAtual");
    if (!estadoAtual)
    {
        cerr << "Elemento EstadoAtual não encontrado" << endl;
        return false;
    }

    estado = vector<vector<string>>(4, vector<string>(4));

    tinyxml2::XMLElement *row = estadoAtual->FirstChildElement("row");
    int rowIndex = 0;

    while (row && rowIndex < 4)
    {
        tinyxml2::XMLElement *col = row->FirstChildElement("col");
        int colIndex = 0;

        while (col && colIndex < 4)
        {
            estado[rowIndex][colIndex] = col->GetText();
            col = col->NextSiblingElement("col");
            colIndex++;
        }

        row = row->NextSiblingElement("row");
        rowIndex++;
    }

    acoes.clear();
    tinyxml2::XMLElement *acoesElement = root->FirstChildElement("Acoes");
    if (acoesElement)
    {
        tinyxml2::XMLElement *acao = acoesElement->FirstChildElement("acao");
        while (acao)
        {
            acoes.push_back(acao->GetText());
            acao = acao->NextSiblingElement("acao");
        }
    }

    resetTempo();

    limparEstadosVisitados();

    return true;
}

// Método para gerar estado inicial aleatório
void EloMaluco::gerarEstadoAleatorio()
{
    vector<string> pecas = {
        "vms", "ams", "vds", "brs", 
        "vmm", "amm", "vdm", "brm", 
        "vmm", "amm", "vdm", "vzo", 
        "vmi", "ami", "vdi", "bri"  
    };

    // Aleatoriza as peças
    random_device rd;
    mt19937 gen(rd());
    std::shuffle(pecas.begin(), pecas.end(), gen);

    int index = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            estado[i][j] = pecas[index++];
        }
    }

    resetTempo();

    acoes.clear();

    limparEstadosVisitados();
}

// Método para exibir o estado atual
void EloMaluco::exibirEstado() const
{
    cout << "\n+---------+---------+---------+---------+\n";
    for (int i = 0; i < 4; i++)
    {
        cout << "|";
        for (int j = 0; j < 4; j++)
        {
            string piece = estado[i][j];
            int padding = (9 - piece.length()) / 2;
            cout << string(padding, ' ') << piece
                 << string(9 - piece.length() - padding, ' ') << "|";
        }
        cout << "\n+---------+---------+---------+---------+\n";
    }
    cout << "Movimentos: " << tempo << endl;
    if (!acoes.empty())
    {
        cout << "Ações: ";
        for (const auto &acao : acoes)
        {
            cout << acao << " ";
        }
        cout << endl;
    }
}

// Método para verificar se o estado atual é solução
bool EloMaluco::ehSolucao() const
{
    const vector<vector<string>> estadoFinal = {
        {"vms", "ams", "vds", "brs"},
        {"vmm", "amm", "vdm", "brm"},
        {"vmm", "amm", "vdm", "vzo"},
        {"vmi", "ami", "vdi", "bri"}};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (estado[i][j] != estadoFinal[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

// Métodos para realizar ações no jogo

//rsd
void EloMaluco::rotacionarSuperiorDireita()
{
    std::rotate(estado[0].rbegin(), estado[0].rbegin() + 1, estado[0].rend());
    tempo++;
}

//rse
void EloMaluco::rotacionarSuperiorEsquerda()
{
    std::rotate(estado[0].begin(), estado[0].begin() + 1, estado[0].end());
    tempo++;
}

//rid
void EloMaluco::rotacionarInferiorDireita()
{
    std::rotate(estado[3].rbegin(), estado[3].rbegin() + 1, estado[3].rend());
    tempo++;
}

//rie
void EloMaluco::rotacionarInferiorEsquerda()
{
    std::rotate(estado[3].begin(), estado[3].begin() + 1, estado[3].end());
    tempo++;
}

//mfc
void EloMaluco::moverFaceAbaixoParaCima()
{
    int vzoRow = -1, vzoCol = -1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (estado[i][j] == "vzo")
            {
                vzoRow = i;
                vzoCol = j;
                break;
            }
        }
        if (vzoRow != -1)
            break;
    }

    if (vzoRow > 0)
    {
        swap(estado[vzoRow][vzoCol], estado[vzoRow - 1][vzoCol]);
        tempo++;
    }
}

//mfb
void EloMaluco::moverFaceAcimaParaBaixo()
{
    int vzoRow = -1, vzoCol = -1;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (estado[i][j] == "vzo")
            {
                vzoRow = i;
                vzoCol = j;
                break;
            }
        }
        if (vzoRow != -1)
            break;
    }

    if (vzoRow < 3)
    {
        swap(estado[vzoRow][vzoCol], estado[vzoRow + 1][vzoCol]);
        tempo++;
    }
}

// Verifica se o estado atual já foi visitado
bool EloMaluco::estadoJaVisitado() const
{
    for (const auto &estadoVisitado : estadosVisitados)
    {
        bool igual = true;
        for (int i = 0; i < 4 && igual; i++)
        {
            for (int j = 0; j < 4 && igual; j++)
            {
                if (estado[i][j] != estadoVisitado[i][j])
                {
                    igual = false;
                }
            }
        }
        if (igual)
            return true;
    }
    return false;
}

// Adiciona o estado atual à lista de estados visitados
void EloMaluco::adicionarEstadoVisitado()
{
    estadosVisitados.push_back(estado);
}

// Limpa a lista de estados visitados
void EloMaluco::limparEstadosVisitados()
{
    estadosVisitados.clear();
}

// Adicionar nova função de cálculo de heurística antes do resolverJogo
int EloMaluco::calcularHeuristica()
{
    const vector<vector<string>> estadoFinal = {
        {"vms", "ams", "vds", "brs"},
        {"vmm", "amm", "vdm", "brm"},
        {"vmm", "amm", "vdm", "vzo"},
        {"vmi", "ami", "vdi", "bri"}};

    int distanciaTotal = 0;

    // Calcula distância Manhattan para cada peça
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (estado[i][j] != "vzo")
            {
                // Encontra posição correta da peça
                for (int fi = 0; fi < 4; fi++)
                {
                    for (int fj = 0; fj < 4; fj++)
                    {
                        if (estadoFinal[fi][fj] == estado[i][j])
                        {
                            distanciaTotal += abs(i - fi) + abs(j - fj);
                        }
                    }
                }
            }
        }
    }
    return distanciaTotal;
}

// Resolver o jogo
bool EloMaluco::resolverJogo()
{
    if (ehSolucao())
    {
        salvarSolucao("EloMaluco_solucao.xml");
        return true;
    }

    estadosVisitados.push_back(estado);
    auto estadoAnterior = estado;
    vector<pair<int, string>> movimentosPossiveis;

    // Tenta todos os movimentos possíveis e calcula suas heurísticas
    auto testarMovimento = [&](const string &movimento, void (EloMaluco::*funcao)())
    {
        (this->*funcao)();
        int heuristica = calcularHeuristica();
        estado = estadoAnterior;
        movimentosPossiveis.push_back(make_pair(heuristica, movimento)); 
    };

    // Testa todos os movimentos possíveis
    testarMovimento("rid", &EloMaluco::rotacionarInferiorDireita);
    testarMovimento("rie", &EloMaluco::rotacionarInferiorEsquerda);
    testarMovimento("rsd", &EloMaluco::rotacionarSuperiorDireita);
    testarMovimento("rse", &EloMaluco::rotacionarSuperiorEsquerda);
    testarMovimento("mfc", &EloMaluco::moverFaceAbaixoParaCima);
    testarMovimento("mfb", &EloMaluco::moverFaceAcimaParaBaixo);

    // Ordena movimentos por heurística (do menor para o maior)
    sort(movimentosPossiveis.begin(), movimentosPossiveis.end());

    // Tenta movimentos em ordem de melhor heurística
    for (const auto &movimento : movimentosPossiveis)
    {
        const string &acao = movimento.second;

        exibirEstado();

        if (acao == "rid")
            rotacionarInferiorDireita();
        else if (acao == "rie")
            rotacionarInferiorEsquerda();
        else if (acao == "rsd")
            rotacionarSuperiorDireita();
        else if (acao == "rse")
            rotacionarSuperiorEsquerda();
        else if (acao == "mfc")
            moverFaceAbaixoParaCima();
        else if (acao == "mfb")
            moverFaceAcimaParaBaixo();

        if (ehSolucao())
        {
            acoes.push_back(acao);
            salvarSolucao("EloMaluco_solucao.xml");
            return true;
        }

        if (!estadoJaVisitado())
        {
            acoes.push_back(acao);
            if (resolverJogo())
            {
                return true;
            }
            acoes.pop_back();
        }
        estado = estadoAnterior;
    }
    return false;
}

// Salva a solução em um arquivo XML
void EloMaluco::salvarSolucao(const string &arquivo)
{
    tinyxml2::XMLDocument doc;

    tinyxml2::XMLDeclaration *decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
    doc.InsertFirstChild(decl);

    tinyxml2::XMLElement *root = doc.NewElement("EloMaluco");
    doc.InsertEndChild(root);

    tinyxml2::XMLElement *estadoAtual = doc.NewElement("EstadoAtual");
    root->InsertEndChild(estadoAtual);

    for (int i = 0; i < 4; i++)
    {
        tinyxml2::XMLElement *row = doc.NewElement("row");
        estadoAtual->InsertEndChild(row);

        for (int j = 0; j < 4; j++)
        {
            tinyxml2::XMLElement *col = doc.NewElement("col");
            col->SetText(estado[i][j].c_str());
            row->InsertEndChild(col);
        }
    }

    if (!acoes.empty())
    {
        tinyxml2::XMLElement *acoesElement = doc.NewElement("Acoes");
        root->InsertEndChild(acoesElement);

        for (const string &acao : acoes)
        {
            tinyxml2::XMLElement *acaoElement = doc.NewElement("acao");
            acaoElement->SetText(acao.c_str());
            acoesElement->InsertEndChild(acaoElement);
        }
    }

    // Tentar salvar arquivo no diretório atual
    string fullPath = arquivo;
    if (arquivo.find('/') == string::npos && arquivo.find('\\') == string::npos)
    {
        fullPath = "./" + arquivo;
    }

    doc.SaveFile(fullPath.c_str());

    if (doc.ErrorID() != 0)
    {
        cerr << "Erro ao salvar arquivo: " << fullPath << endl;
        cerr << "Erro: " << doc.ErrorStr() << endl;
    }
}

int EloMaluco::getTempoMovimentos() const
{
    return tempo;
}

void EloMaluco::resetTempo()
{
    tempo = 0;
}