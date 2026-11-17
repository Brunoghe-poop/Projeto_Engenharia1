#include "Paciente.h"

#include <cctype>

Paciente::Paciente() : id_(0), nome_(), cpf_(), telefone_(), acesso_(nullptr) {}

Paciente::Paciente(int id, const std::string& nome, const std::string& cpf,
                   const std::string& telefone)
    : id_(id), nome_(nome), cpf_(cpf), telefone_(telefone), acesso_(nullptr) {}

Paciente::Paciente(const Paciente& outro)
    : id_(outro.id_), nome_(outro.nome_), cpf_(outro.cpf_),
      telefone_(outro.telefone_),
      acesso_(outro.acesso_ ? new Usuario(*outro.acesso_) : nullptr) {}

Paciente& Paciente::operator=(const Paciente& outro) {
    if (this != &outro) {
        // aloca primeiro: se new lancar, o objeto continua integro
        Usuario* novoAcesso = outro.acesso_ ? new Usuario(*outro.acesso_) : nullptr;
        delete acesso_;
        acesso_ = novoAcesso;
        id_ = outro.id_;
        nome_ = outro.nome_;
        cpf_ = outro.cpf_;
        telefone_ = outro.telefone_;
    }
    return *this;
}

Paciente::~Paciente() { delete acesso_; }

int Paciente::getId() const { return id_; }
const std::string& Paciente::getNome() const { return nome_; }
const std::string& Paciente::getCpf() const { return cpf_; }
const std::string& Paciente::getTelefone() const { return telefone_; }
const Usuario* Paciente::getAcesso() const { return acesso_; }

void Paciente::setId(int id) { id_ = id; }

void Paciente::setAcesso(const Usuario& usuario) {
    Usuario* novo = new Usuario(usuario);
    delete acesso_;
    acesso_ = novo;
}

void Paciente::removerAcesso() {
    delete acesso_;
    acesso_ = nullptr;
}

std::string Paciente::normalizarCpf(const std::string& cpf) {
    std::string digitos;
    for (char c : cpf) {
        if (std::isdigit(static_cast<unsigned char>(c))) digitos += c;
    }
    return digitos;
}
