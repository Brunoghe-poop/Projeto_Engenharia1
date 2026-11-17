#include "Consulta.h"
//construtor 1
Consulta::Consulta()
    : id_(0), dataHora_(), duracaoMin_(30), status_(StatusConsulta::Agendada),
      paciente_(), medico_(), registro_(nullptr) {}
//construtor 2
Consulta::Consulta(const Paciente& paciente, const Medico& medico,
                   const DataHora& dataHora, int duracaoMin)
    : id_(0), dataHora_(dataHora), duracaoMin_(duracaoMin),
      status_(StatusConsulta::Agendada), paciente_(paciente), medico_(medico),
      registro_(nullptr) {}

//construtor 3
Consulta::Consulta(const Consulta& outra)
    : id_(outra.id_), dataHora_(outra.dataHora_), duracaoMin_(outra.duracaoMin_),
      status_(outra.status_), paciente_(outra.paciente_), medico_(outra.medico_),
      registro_(outra.registro_ ? new RegistroClinico(*outra.registro_) : nullptr) {}

// destrutor
Consulta::~Consulta() { delete registro_; }

//getters
int Consulta::getId() const { return id_; }
const DataHora& Consulta::getDataHora() const { return dataHora_; }
const DataHora& Consulta::getHorario() const { return dataHora_; }
int Consulta::getDuracaoMin() const { return duracaoMin_; }
StatusConsulta Consulta::getStatus() const { return status_; }
std::string Consulta::getStatusStr() const { return paraString(status_); }
const Medico& Consulta::getMedico() const { return medico_; }
const Paciente& Consulta::getPaciente() const { return paciente_; }
const std::string& Consulta::getNomePaciente() const { return paciente_.getNome(); }
const RegistroClinico* Consulta::getRegistro() const { return registro_; }

// setters
void Consulta::setId(int id) { id_ = id; }
void Consulta::setStatus(StatusConsulta status) { status_ = status; }

// copia dados de uma consulta
Consulta& Consulta::operator=(const Consulta& outra) {
    if (this != &outra) {
        RegistroClinico* novoRegistro =
            outra.registro_ ? new RegistroClinico(*outra.registro_) : nullptr;
        delete registro_;
        registro_ = novoRegistro;
        id_ = outra.id_;
        dataHora_ = outra.dataHora_;
        duracaoMin_ = outra.duracaoMin_;
        status_ = outra.status_;
        paciente_ = outra.paciente_;
        medico_ = outra.medico_;
    }
    return *this;
}

RegistroClinico& Consulta::obterOuCriarRegistro() {
    if (registro_ == nullptr) registro_ = new RegistroClinico();
    return *registro_;
}



