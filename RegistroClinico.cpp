#include "RegistroClinico.h"

RegistroClinico::RegistroClinico() : diagnostico_(), prescricao_() {}

RegistroClinico::RegistroClinico(const RegistroClinico& outro)
    : diagnostico_(outro.diagnostico_), prescricao_(outro.prescricao_) {}

RegistroClinico& RegistroClinico::operator=(const RegistroClinico& outro) {
    if (this != &outro) {
        diagnostico_ = outro.diagnostico_;
        prescricao_ = outro.prescricao_;
    }
    return *this;
}

RegistroClinico::~RegistroClinico() {}

void RegistroClinico::setDiagnostico(const std::string& texto) { diagnostico_ = texto; }
void RegistroClinico::setPrescricao(const std::string& texto) { prescricao_ = texto; }
const std::string& RegistroClinico::getDiagnostico() const { return diagnostico_; }
const std::string& RegistroClinico::getPrescricao() const { return prescricao_; }
