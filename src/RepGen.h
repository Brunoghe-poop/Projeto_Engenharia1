#ifndef REPGEN_H
#define REPGEN_H

#include "Consulta.h"
#include "DataHora.h"
#include "ErroHealthSync.h"
#include "Paciente.h"
#include <cstddef>
#include <expected>
#include <mutex>
#include <optional>
#include <vector>

// -----------------------------------------------------------------------------
// RepGen: repositorio em memoria. As operacoes de escrita sao atomicas (mutex),
// simulando a transacao ACID usada para impedir conflitos de horario.
// -----------------------------------------------------------------------------
class RepGen {
public:
    RepGen();
    RepGen(const RepGen& outro);
    RepGen& operator=(const RepGen& outro);
    ~RepGen();

    // atribui id e grava; CpfDuplicado se o CPF ja existe
    std::expected<Paciente, ErroHealthSync> inserirPaciente(const Paciente& p);

    // verifica o conflito (medico + horario) e grava na MESMA secao critica
    std::expected<Consulta, ErroHealthSync> inserirConsultaSeLivre(
        const Consulta& c);

    std::optional<Paciente> buscarPaciente(int id) const;
    std::vector<Paciente> listarPacientes() const;

    std::optional<Consulta> buscarConsulta(int id) const;
    bool atualizarConsulta(const Consulta& c);
    std::vector<Consulta> listarConsultasDoDia(const DataHora& dia) const;

    // Falha de infraestrutura (servico sem repositorio): lanca std::runtime_error
    static RepGen& exigir(RepGen* repo);

    std::size_t totalPacientes() const;
    std::size_t totalConsultas() const;

private:
    mutable std::mutex mtx_;
    std::vector<Paciente> pacientes_;
    std::vector<Consulta> consultas_;
    int proximoIdPaciente_;
    int proximoIdConsulta_;
};

#endif  // REPGEN_H
