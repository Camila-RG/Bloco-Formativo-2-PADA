#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

const int infinito = 999999;

int main() {
    int n = 6;
    vector<char> nomes = {'A','B','C','D','E','F'};

    vector<vector<int>> grafo(n, vector<int>(n, infinito));

    // abre o arquivo com o grafo
    ifstream arquivo("grafo");
    if (!arquivo.is_open()) {
        cout << "nao deu pra abrir o arquivo =( \n";
        return 1;
    }

    string linha;
    char de = ' '; // de = ponto que começa o caminho 
    while (getline(arquivo, linha)) {
        // pega a letra que aparece no começo da linha (origem)
        if (linha.size() > 2 && linha[2] == '"') {
            de = linha[3]; // sempre maiuscula
        }

        // procura destino e custo da aresta entre dois nos
        if (linha.find(":") != string::npos) { // o find procura se existe o caractere
            char para = ' '; // para = ponto que termina
            int custo = 0;

            // acha a letra do destino
            for (int i = 0; i < (int)linha.size(); i++) {
                if (linha[i] >= 'A' && linha[i] <= 'F') {
                    para = linha[i];
                }
            }

            // pega os números depois dos dois pontos
            size_t pos = linha.find(":");
            if (pos != string::npos) {
                string num = linha.substr(pos+1);
                // tira espaços e vírgulas
                while (!num.empty() && (num[0] == ' ' || num[0] == ',')) {
                    num.erase(0,1);
                }
                // converte manualmente string para número
                for (int i = 0; i < (int)num.size(); i++) {
                    if (num[i] >= '0' && num[i] <= '9') {
                        custo = custo * 10 + (num[i] - '0');
                    }
                }
            }

            if (de != ' ' && para != ' ' && custo > 0) {
                int iDe = de - 'A';
                int iPara = para - 'A';
                grafo[iDe][iPara] = custo;
            }
        }
    }
    arquivo.close();

    // copia valores para os dois lados (grafo nao-direcionado)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grafo[i][j] != infinito) {
                grafo[j][i] = grafo[i][j];
            }
        }
    }

    // laço de consultas
    while (true) {
        char origemChar, destinoChar;
        cout << "\nDigite origem (A-F) ou X para sair: ";
        cin >> origemChar;
        if (origemChar == 'X') break;

        cout << "Digite destino (A-F): ";
        cin >> destinoChar;

        int origem = origemChar - 'A';
        int destino = destinoChar - 'A';

        vector<int> dist(n, infinito);
        vector<int> visitado(n, 0);
        vector<int> anterior(n, -1);

        dist[origem] = 0;

        // algoritmo de Dijkstra
        for (int k = 0; k < n; k++) {
            int u = -1;
            for (int i = 0; i < n; i++) {
                if (!visitado[i] && (u == -1 || dist[i] < dist[u])) {
                    u = i;
                }
            }

            if (u == -1 || dist[u] == infinito) break;
            visitado[u] = 1;

            for (int v = 0; v < n; v++) {
                if (grafo[u][v] != infinito && dist[u] + grafo[u][v] < dist[v]) {
                    dist[v] = dist[u] + grafo[u][v];
                    anterior[v] = u;
                }
            }
        }

        if (dist[destino] == infinito) {
            cout << "\nNao existe caminho!\n";
        } else {
            cout << "\nCusto minimo: " << dist[destino] << "\n";
            cout << "Caminho: ";

            vector<int> caminho;
            for (int v = destino; v != -1; v = anterior[v]) {
                caminho.push_back(v);
            }

            for (int i = caminho.size() - 1; i >= 0; i--) {
                cout << nomes[caminho[i]];
                if (i > 0) cout << "->";
            }
            cout << "\n";
        }
    }

    cout << "\nFim do programa\n";
    return 0;
}
