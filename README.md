# Tabela-Hash
[hashtable] Implementação de tabela hash usando endereçamento aberto + template

Implementação de uma tabela hash com tratamento de colisão por endereçamento aberto (open addressing) usando templates.

Como armazenaremos na tabela não apenas as chaves mas também os valores associados a cada chave, cada elemento
do vector deve ter a capacidade de armazenar um tipo de dado composto (chave,valor). Como tipo de dado composto,
usamos o tipo de dado std::pair do C++. Além disso, cada elemento do vector deve guardar também seu status, que pode ser: EMPTY,  ACTIVE ou DELETED. 

A fim de representar esses três valores, um tipo chamado Status pode ser definido como a seguir: </br>
// Definicao do Tipo de dado Status, que pode assumir 3 valores </br>
``` enum Status { ACTIVE, EMPTY, DELETED }; ```

Dados esses requisitos, cada elemento desse std::vector é um objeto do tipo HashEntry, definido como a seguir: </br>
// Tipo de dado da tabela hash
```
    struct HashEntry{
        std::pair<Tkey,Tvalue> element; // guarda o par (chave,valor)
        Status status; // status, que pode ser ACTIVE, EMPTY ou DELETED
        // Construtor do struct HashEntry
        HashEntry(){
            element = make_pair(Tkey{ },Tvalue{ });
            status = EMPTY;
        }
    };
```

Testes encontram-se na pasta 'testes'.
