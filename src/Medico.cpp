#include "Medico.h"

Medico::Medico() : Usuario(), expediente_() {}

Medico::Medico(int id, const std::string& nome, const std::string& expediente)
    : Usuario(id, nome, Perfil::Medico), expediente_(expediente) {}

Medico::Medico(const Medico& outro)
    : Usuario(outro), expediente_(outro.expediente_) {}

Medico& Medico::operator=(const Medico& outro) {
    if (this != &outro) {
        Usuario::operator=(outro);  // parte herdada
        expediente_ = outro.expediente_;
    }
    return *this;
}

Medico::~Medico() {}

const std::string& Medico::getExpediente() const { return expediente_; }
