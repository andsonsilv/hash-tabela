#ifndef hashtableprobe_h
#define hashtableprobe_h
#include <iostream>
#include <utility> // para usar a funcao make_pair() e o tipo std::pair
#include <functional> // para usar a classe std::hash
#include <vector>

using std::hash;

// Definicao do Tipo de dado Status, que pode assumir 3 valores
enum Status { ACTIVE, EMPTY, DELETED };

/* Declaracao do template de classe HashTable
 *     Esse template tem dois tipos genericos:
 *     Tkey: tipo da chave Tvalue: tipo do valor associado a chave */
template <typename Tkey, typename Tvalue>
class HashTable {
public:
    // Construtor. A tabela deve ter sempre um tamanho primo
    HashTable( int initialSize = 101 );

    /* Devolve 'true' se a chave x estiver na tabela; e 'false' caso contrario
     * No caso em que devolve 'true', a variavel 'result' deve conter o valor associado a chave.
     * Assim, esse valor podera ser resgatado fora desta funcao-membro.
     * No caso em que devolve 'false', o valor da variavel 'result' pode ser qualquer coisa (lixo) */
    bool search( const Tkey &x, Tvalue &result );

    /* Insere o par (chave,valor) na tabela e devolve 'true' em caso de sucesso;
     * devolve 'false' se nao for possivel inserir
     * Logo apos inserir um elemento na tabela, essa funcao deve checar o
     * fator de carga: currentSize/table.size().
     * Se esse valor for maior que 0.5 entao a funcao rehashing deve ser chamada */
    bool insert( const Tkey &x, const Tvalue &v );

    // Remove chave x da tabela e devolve se a remocao foi bem sucedida
    bool remove( const Tkey &x );

    // Deixa a tabela vazia, sem elementos
    void clear();

    // Devolve o numero de elementos na tabela
    int size() const;

    // Imprime a tabela
    void print() const;

private:
    // Tipo de dado da tabela hash
    struct HashEntry{
        std::pair<Tkey,Tvalue> element; // guarda o par (chave,valor)
        Status status; // status, que pode ser ACTIVE, EMPTY ou DELETED

        // Construtor do struct HashEntry
        HashEntry(){
            element = make_pair(Tkey{ },Tvalue{ });
            status = EMPTY;
        }
    };

    std::vector<HashEntry> table; // tabela hash

    int currentSize; // numero de chaves armazenadas na tabela

    // funcao de hashing que usa o metodo de sondagem linear
    size_t hashFunction(const Tkey &x, int i) const;

    /* Funcao de rehashing
     * Redimensiona a tabela hash para que ela tenha um tamanho primo maior que o dobro do
     * tamanho da tabela atual. Por exemplo, se a tabela tinha 7 slots, ela passara a ter agora
     * 17 slots, pois 17 eh o proximo primo maior que 14 = 7 * 2.
     * Cada elemento deve ser reinserido na tabela. Uma consequencia disso eh que
     * alguns elementos mudarao de slot ja que a tabela mudou de tamanho */
    void rehash();
};

/* Funcao auxiliar
    devolve 'true'se n eh primo; devolve 'false' caso contrario */
bool isPrime (int n){
    if (n == 2 || n == 3)
        return true;
    if (n == 1 || n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return false;
    return true;
}

/* Funcao auxiliar
    devolve o proximo primo maior ou igual a n */
int nextPrime(int n){
    if( n % 2 == 0 ) n++;
    while( !isPrime(n) ) n += 2;
    return n;
}

template <typename Tkey, typename Tvalue>
void HashTable<Tkey,Tvalue>::print() const{
    for(int i = 0; i < table.size(); ++i){
        std::cout << "T[" << i << "]:";
        if(table[i].status == ACTIVE)
            std::cout << "(" << table[i].element.first << "," << table[i].element.second << ")";
        std::cout << std::endl;
    }
}

template <typename Tkey, typename Tvalue>
HashTable<Tkey,Tvalue>::HashTable(int initialSize){
    int novoPrimo = (initialSize != 101) ? nextPrime(initialSize) : initialSize; // calcula um primo
    table.resize(novoPrimo); // redimensiona a tabela
    currentSize = 0; // a tabela inicia vazia
}

template <typename Tkey, typename Tvalue>
void HashTable<Tkey,Tvalue>::clear(){
    for(int i = 0; i < table.size(); i++){
        table[i].status = EMPTY;
    }
    currentSize = 0;
}

template <typename Tkey, typename Tvalue>
size_t HashTable<Tkey,Tvalue>::hashFunction(const Tkey &x, int i ) const{
    hash<Tkey> hf;
    return (hf(x) % table.size() + i) % table.size();
}

template <typename Tkey, typename Tvalue>
int HashTable<Tkey,Tvalue>::size() const{
    return currentSize;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey,Tvalue>::search(const Tkey &x, Tvalue &result ){
    int i = 0;
    int j;
    do {
        j = hashFunction(x,i);
        if(table[j].status == ACTIVE and table[j].element.first == x){
            result = table[j].element.second;
            return true;
        }
        i++;
    } while(table[j].status != EMPTY and i < table.size());
    return false;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey,Tvalue>::insert(const Tkey &x, const Tvalue &v ){
    int i = 0;
    int j;
    do{
        j = hashFunction(x, i);
        if(table[j].status == EMPTY || table[j].status == DELETED){
            table[j].element.first = x;
            table[j].element.second = v;
            table[j].status = ACTIVE;
            currentSize++;
            if(static_cast<float>(currentSize)/table.size() > 0.5)
                this->rehash();
            return true;
        }
        i++;
    } while( i < table.size());
    return false;
}

template <typename Tkey, typename Tvalue>
bool HashTable<Tkey,Tvalue>::remove(const Tkey &x){
    int i = 0;
    int j;
    do{
        j = hashFunction(x,i);
        if(table[j].status == ACTIVE && table[j].element.first == x){
            table[j].status = DELETED;
            currentSize--;
            return true;
        }
        i++;
    } while(table[j].status != EMPTY && i < table.size());
    return false;
}

template <typename Tkey, typename Tvalue>
void HashTable<Tkey,Tvalue>::rehash(){
    auto table2 = table;
    int size_table2 = nextPrime(table.size() * 2);
    clear();
    table.resize(size_table2);
    for(int i = 0; i < table2.size(); i++){
        if(table2[i].status ==  ACTIVE)
            insert(table2[i].element.first,table2[i].element.second);
    }
}

#endif
