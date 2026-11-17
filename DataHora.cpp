#include "DataHora.h"

#include <cstdio>
#include <ctime>

namespace {

int diasNoMes(int ano, int mes) {
    static const int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    //anos bissextos
    const bool bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);


    return (mes == 2 && bissexto) ? 29 : dias[mes - 1];
}

}  // namespace para "esconder" a função e evitar uso em outros arquivos

// construtor 1
DataHora::DataHora() : ano_(0), mes_(0), dia_(0), hora_(0), minuto_(0) {}

// construtor 2
DataHora::DataHora(int ano, int mes, int dia, int hora, int minuto)
    : ano_(ano), mes_(mes), dia_(dia), hora_(hora), minuto_(minuto) {}

// construtor 3
DataHora::DataHora(const std::string& texto)
    : ano_(0), mes_(0), dia_(0), hora_(0), minuto_(0) {
    int a, me, d, h, mi;
    if (std::sscanf(texto.c_str(), "%d-%d-%d %d:%d", &a, &me, &d, &h, &mi) == 5) {
        ano_ = a; mes_ = me; dia_ = d; hora_ = h; minuto_ = mi;
    }  // texto mal formatado -> permanece invalida
}

// construtor 4
DataHora::DataHora(const DataHora& outra)
    : ano_(outra.ano_), mes_(outra.mes_), dia_(outra.dia_),
      hora_(outra.hora_), minuto_(outra.minuto_) {}

// destrutor
DataHora::~DataHora() {}

// getters
int DataHora::getAno() const { return ano_; }
int DataHora::getMes() const { return mes_; }
int DataHora::getDia() const { return dia_; }
int DataHora::getHora() const { return hora_; }
int DataHora::getMinuto() const { return minuto_; }


// operador para cópia
DataHora& DataHora::operator=(const DataHora& outra) {
    if (this != &outra) {
        ano_ = outra.ano_;
        mes_ = outra.mes_;
        dia_ = outra.dia_;
        hora_ = outra.hora_;
        minuto_ = outra.minuto_;
    }
    return *this;
}

// definicao de variavel privada
long long DataHora::chave() const {
    return (((static_cast<long long>(ano_) * 100 + mes_) * 100 + dia_) * 100 +
            hora_) * 100 + minuto_;
}

// booleanos para verificacao e validacao
bool DataHora::ehValida() const {
    if (ano_ < 1 || mes_ < 1 || mes_ > 12) return false;
    if (dia_ < 1 || dia_ > diasNoMes(ano_, mes_)) return false;
    return hora_ >= 0 && hora_ <= 23 && minuto_ >= 0 && minuto_ <= 59;
}

bool DataHora::ehPassado() const {
    const std::time_t agora = std::time(nullptr);
    const std::tm* t = std::localtime(&agora);
    const DataHora atual(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                         t->tm_hour, t->tm_min);
    return chave() < atual.chave();
}

bool DataHora::mesmoDia(const DataHora& outra) const {
    return ano_ == outra.ano_ && mes_ == outra.mes_ && dia_ == outra.dia_;
}

std::string DataHora::str() const {
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d",
                  ano_, mes_, dia_, hora_, minuto_);
    return buffer;
}

// operadores para manipulacao
bool DataHora::operator==(const DataHora& outra) const {
    return chave() == outra.chave();
}

bool DataHora::operator<(const DataHora& outra) const {
    return chave() < outra.chave();
}
