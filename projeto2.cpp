// Copyright [2024] <Leonardo Dias Gutterres>

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>

using namespace std;

// Classe que representa um nó na trie
class TrieNode {
public:
    unordered_map<char, TrieNode*> children; // Mapeia caracteres para os filhos
    bool isEndOfWord; // Indica se o nó é o fim de uma palavra
    int startPos; // Posição inicial da palavra no arquivo
    int lineLength; // Comprimento da linha onde a palavra foi definida

    // Construtor
    TrieNode() : isEndOfWord(false), startPos(-1), lineLength(0) {}
};

// Classe que representa a trie
class Trie {
private:
    TrieNode* root; // Raiz da trie

public:
    // Construtor
    Trie() {
        root = new TrieNode();
    }

    // Método para inserir uma palavra na trie com sua posição e comprimento da linha
    void insert(const string& word, int startPos, int lineLength) {
        TrieNode* current = root;  //  Variável para percorrer a árvore
        for (char ch : word) {  // Itera sobre os caracteres da palavra recebida
            if (current->children.find(ch) == current->children.end()) {  //  Se o nó atual não tiver um filho correspondente ao proximo caractere:
                current->children[ch] = new TrieNode();  //  Cria um novo nó como filho desse nó atual
            }
            current = current->children[ch];  //  Atualiza o nó para percorrer a árvore
        }

        //  Definição dos atributos do novo nó criado, que corresponde ao término de uma palavra completa
        current->isEndOfWord = true;
        current->startPos = startPos;
        current->lineLength = lineLength;
    }

    // Método para contar o número de palavras que começam com um determinado prefixo
    int countWordsWithPrefix(const string& prefix) {
        TrieNode* current = root;  //  Variável para percorrer a árvore
        for (char ch : prefix) {  //  Itera sobre os caracteres do prefixo recebido
            if (current->children.find(ch) == current->children.end()) {  //  Verifica se o caractere atual corresponde a uma sequência da trie
                return 0;  //  Se não corresponder, retorna 0
            }
            current = current->children[ch];  //  Atualiza o nó para percorrer a árvore
        }
        return countWords(current);
    }

    // Método para obter a posição e o comprimento da linha de uma palavra
    pair<int, int> getPositionAndLength(const string& word) {
        TrieNode* current = root;  //  Variável para percorrer a árvore
        for (char ch : word) {  //  Itera sobre os caracteres da palavra recebida
            if (current->children.find(ch) == current->children.end()) {
                return {-1, 0}; // Palavra não encontrada
            }
            current = current->children[ch];  //  Atualiza o nó para percorrer a árvore
        }
        return {current->startPos, current->lineLength};  //  Retorna o pair contendo as informações com a posição da definição da palavra no arquivo e o comprimento da linha em que ela se encontra
    }

private:
    // Método auxiliar para contar o número de palavras a partir de um nó
    int countWords(TrieNode* node) {  //  Inicialmente, recebe o nó correspondente ao ultimo caractere do prefixo
        if (node == nullptr) {
            return 0;
        }

        int count = 0;
        if (node->isEndOfWord) {
            count++;  //  Aumenta a quantidade de palavras se a sequência de caracteres atual resultar no fim de uma palavra
        }

        for (auto& child : node->children) {  //  Itera sobre os filhos do nó
            count += countWords(child.second);  //  O método é chamado recursivamente para cada filho, aumentando "count" em 1 para cada vez que o fim de uma palavra for encontrado
        }

        return count;
    }
};

int main() {
    char filename[100];
    cin >> filename;  // Entrada do nome do arquivo a ser analisado

    ifstream file(filename);
    if (!file.is_open()) {  // Verifica se o arquivo foi aberto corretamente
        cout << "erro" << endl;
        return 0;
    }

    Trie trie;  // Árvore trie
    string word;  //  Palavra de entrada para análise
    string line;  //  Linha do arquivo
    int startPos = 0;  //  Variável para guardar a posição do início da declaração de uma palavra
    int lineLength = 0;  //  Variável para guardar o comprimento da linha onde a palavra aparece

    // Ler e inserir palavras na trie
    while (getline(file, line)) {  //  Itera sobre as linhas do arquivo
        size_t start = line.find('[');
        size_t end = line.find(']');
        if (start != string::npos && end != string::npos && end > start + 1) {  //  Verifica se os colchetes estão aninhados corretamente
            string dict_word = line.substr(start + 1, end - start - 1);  // Extrai a palavra entre os delimitadores '[' e ']'
            trie.insert(dict_word, startPos, line.length());  //  Insere a palavra na Trie com seus atributos já definidos conforme a leitura do arquivo
        }
        startPos += line.length() + 1; // Atualiza a variável que guarda a posição de início de uma palavra, considerando também o caractere de mudança de linha
    }

    file.close();  //  Fecha o arquivo

    // Ler palavras de entrada até encontrar "0"
    while (true) {
        cin >> word;
        if (word == "0") {
            break;
        }

        int count = trie.countWordsWithPrefix(word);
        if (count > 0) {  //  Imprime as informações necessárias se a string corresponder a um prefixo
            cout << word << " is prefix of " << count << " words" << endl;
            std::tie(startPos, lineLength) = trie.getPositionAndLength(word);  // Atualiza as variáveis para corresponder a string atual enviada pelo usuário
            if (startPos != -1) {  //  Se a string corresponder a uma palavra completa, imprime posição de início e comprimento da linha
                cout << word << " is at (" << startPos << "," << lineLength << ")" << endl;
            }
        } else {
            cout << word << " is not prefix" << endl;  //  Imprime se não corresponder a um prefixo
        }
    }

    return 0;  //  Fim do programa
}
