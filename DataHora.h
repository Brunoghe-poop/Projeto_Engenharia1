#ifndef DATAHORA_H
#define DATAHORA_H

#include <string>

class DataHora {
private:
    long long chave() const;  // AAAAMMDDHHMM, permite comparar cronologicamente
    int ano_, mes_, dia_, hora_, minuto_;

public:
    // construtor 1
    DataHora();  // data invalida (0000-00-00 00:00)

    // construtor 2
    DataHora(int ano, int mes, int dia, int hora, int minuto);

    // construtor 3
    explicit DataHora(const std::string& texto);  // "AAAA-MM-DD HH:MM"

    // construtor 4
    DataHora(const DataHora& outra);

    // destrutor
    ~DataHora();

    // getters
    int getAno() const;
    int getMes() const;
    int getDia() const;
    int getHora() const;
    int getMinuto() const;

    // operador de cópia de DataHora
    DataHora& operator=(const DataHora& outra);
    
    // booleanos de verificacao
    bool ehValida() const;
    bool ehPassado() const;                      // compara com o relogio do sistema
    bool mesmoDia(const DataHora& outra) const;  // ignora hora e minuto
    std::string str() const;                   // "AAAA-MM-DD HH:MM"

    // definição de operadores para relacionamento direto
    bool operator==(const DataHora& outra) const;
    bool operator<(const DataHora& outra) const;
};

#endif  // DATAHORA_H
