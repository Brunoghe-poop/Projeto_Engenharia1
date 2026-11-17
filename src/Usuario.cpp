#include "Usuario.h"

Usuario::Usuario() : id_(0), nome_(), email_(), perfil_(Perfil::Paciente) {}

Usuario::Usuario(int id, const std::string& nome, Perfil perfil,
                 const std::string& email)
    : id_(id), nome_(nome), email_(email), perfil_(perfil) {}

Usuario::Usuario(const Usuario& outro)
    : id_(outro.id_), nome_(outro.nome_), email_(outro.email_),
      perfil_(outro.perfil_) {}

Usuario& Usuario::operator=(const Usuario& outro) {
    if (this != &outro) {
        id_ = outro.id_;
        nome_ = outro.nome_;
        email_ = outro.email_;
        perfil_ = outro.perfil_;
    }
    return *this;
}

Usuario::~Usuario() {}

int Usuario::getId() const { return id_; }
const std::string& Usuario::getNome() const { return nome_; }
const std::string& Usuario::getEmail() const { return email_; }
Perfil Usuario::getPerfil() const { return perfil_; }
