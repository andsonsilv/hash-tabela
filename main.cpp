#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "hashtable.h"

using namespace std;

int main(){

    HashTable<string, string> hts(101); // Tabela hash formada por pares (string,string)

    while(true) {
        string cmd;
        getline(cin, cmd);

        stringstream ss{ cmd };
        vector<string> tokens;
        string buffer;

        while(ss >> buffer)
            tokens.push_back(buffer);

        // exit
        if(tokens[0] == "exit") {
            cout << "saindo..." << endl;
            break;
        }
        // insert n
        else if(tokens[0] == "insert") {
            int n = std::stoi(tokens[1]);
            for(int i = 1; i <= n; ++i) {
                string line, key, word;
                getline(cin, line);
                stringstream ss { line };
                ss >> key;
                ss >> word;
                hts.insert(key, word);
            }
        }
        // print
        else if(tokens[0] == "print") {
            hts.print();
        }
        // clear
        else if(tokens[0] == "clear") {
            hts.clear();
            cout << "tabela esvaziada" << endl;
        }
        // size
        else if(tokens[0] == "size") {
            cout << "total de chaves = " << hts.size() << endl;
        }
        // remove
        else if(tokens[0] == "remove") {
            string line, key;
            getline(cin, line);
            stringstream ss { line };
            while(ss >> key)
                hts.remove( key );
        }
        // search k
        else if(tokens[0] == "search") {
            string key = tokens[1];
            string valor;
            if(hts.search(key, valor))
                cout << "(" << key << "," << valor << ")" << endl;
            else
                cout << "chave inexistente" << endl;
        }
    }

    return 0;

}
