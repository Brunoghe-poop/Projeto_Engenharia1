#include "RepGen.h"

#include <stdexcept>

RepGen::RepGen()
    : mtx_(), pacientes_(), consultas_(), proximoIdPaciente_(1),
      proximoIdConsulta_(1) {}

RepGen::RepGen(const RepGen& outro)
    : mtx_(), pacientes_(), consultas_(), proximoIdPaciente_(1),
      proximoIdConsulta_(1) {
    std::lock_guard<std::mutex> trava(outro.mtx_);  // o mutex NAO e copiado
    pacientes_ = outro.pacientes_;
    consultas_ = outro.consultas_;
    proximoIdPaciente_ = outro.proximoIdPaciente_;
    proximoIdConsulta_ = outro.proximoIdConsulta_;
}

RepGen& RepGen::operator=(const RepGen& outro) {
    if (this != &outro) {
        std::scoped_lock trava(mtx_, outro.mtx_);  // evita deadlock
        pacientes_ = outro.pacientes_;
        consultas_ = outro.consultas_;
        proximoIdPaciente_ = outro.proximoIdPaciente_;
        proximoIdConsulta_ = outro.proximoIdConsulta_;
    }
    return *this;
}

RepGen::~RepGen() {}

std::expected<Paciente, ErroHealthSync> RepGen::inserirPaciente(const Paciente& p) {
    std::lock_guard<std::mutex> trava(mtx_);
    const std::string cpf = Paciente::normalizarCpf(p.getCpf());
    for (const Paciente& existente : pacientes_) {
        if (Paciente::normalizarCpf(existente.getCpf()) == cpf) {
            return std::unexpected(ErroHealthSync::CpfDuplicado);
        }
    }
    Paciente novo(p);
    novo.setId(proximoIdPaciente_++);
    pacientes_.push_back(novo);
    return novo;
}

std::expected<Consulta, ErroHealthSync> RepGen::inserirConsultaSeLivre(
    const Consulta& c) {
    std::lock_guard<std::mutex> trava(mtx_);  // verificar + gravar = atomico
    for (const Consulta& existente : consultas_) {
        if (existente.getStatus() != StatusConsulta::Cancelada &&
            existente.getMedico().getId() == c.getMedico().getId() &&
            existente.getDataHora() == c.getDataHora()) {
            return std::unexpected(ErroHealthSync::HorarioOcupado);
        }
    }
    Consulta nova(c);
    nova.setId(proximoIdConsulta_++);
    consultas_.push_back(nova);
    return nova;
}

std::optional<Consulta> RepGen::buscarConsulta(int id) const {
    std::lock_guard<std::mutex> trava(mtx_);
    for (const Consulta& c : consultas_) {
        if (c.getId() == id) return c;
    }
    return std::nullopt;
}

bool RepGen::atualizarConsulta(const Consulta& c) {
    std::lock_guard<std::mutex> trava(mtx_);
    for (Consulta& existente : consultas_) {
        if (existente.getId() == c.getId()) {
            existente = c;
            return true;
        }
    }
    return false;
}

std::vector<Consulta> RepGen::listarConsultasDoDia(const DataHora& dia) const {
    std::lock_guard<std::mutex> trava(mtx_);
    std::vector<Consulta> resultado;
    for (const Consulta& c : consultas_) {
        if (c.getDataHora().mesmoDia(dia)) resultado.push_back(c);
    }
    return resultado;
}

std::size_t RepGen::totalPacientes() const {
    std::lock_guard<std::mutex> trava(mtx_);
    return pacientes_.size();
}

std::size_t RepGen::totalConsultas() const {
    std::lock_guard<std::mutex> trava(mtx_);
    return consultas_.size();
}

std::optional<Paciente> RepGen::buscarPaciente(int id) const {
    std::lock_guard<std::mutex> trava(mtx_);
    for (const Paciente& p : pacientes_) {
        if (p.getId() == id) return p;
    }
    return std::nullopt;
}

std::vector<Paciente> RepGen::listarPacientes() const {
    std::lock_guard<std::mutex> trava(mtx_);
    return pacientes_;
}

RepGen& RepGen::exigir(RepGen* repo) {
    if (repo == nullptr) {
        throw std::runtime_error("Servico sem repositorio associado");
    }
    return *repo;
}
