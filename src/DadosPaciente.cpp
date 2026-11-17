#include "DadosPaciente.h"
// construtor 1
DadosPaciente::DadosPaciente() : nome_(), cpf_(), telefone_() {}

// construtor 2
DadosPaciente::DadosPaciente(const std::string& nome, const std::string& cpf,
                             const std::string& telefone)
    : nome_(nome), cpf_(cpf), telefone_(telefone) {}

// construtor 3
DadosPaciente::DadosPaciente(const DadosPaciente& outros)
    : nome_(outros.nome_), cpf_(outros.cpf_), telefone_(outros.telefone_) {}

// destrutor
DadosPaciente::~DadosPaciente() {}

// getters
const std::string& DadosPaciente::getNome() const { return nome_; }
const std::string& DadosPaciente::getCpf() const { return cpf_; }
const std::string& DadosPaciente::getTelefone() const { return telefone_; }

// função de cópia de dados
DadosPaciente& DadosPaciente::operator=(const DadosPaciente& outros) {
    if (this != &outros) {
        nome_ = outros.nome_;
        cpf_ = outros.cpf_;
        telefone_ = outros.telefone_;
    }
    return *this;
}
