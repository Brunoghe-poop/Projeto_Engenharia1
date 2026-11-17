// assert() so funciona sem NDEBUG; garantimos isso para os testes nunca
// "passarem em branco" caso o Makefile seja alterado para -DNDEBUG.
#undef NDEBUG
#include <cassert>
#include <iostream>

#include "funcional_tests.h"
#include "Consulta.h"
#include "DadosPaciente.h"
#include "DataHora.h"
#include "ErroHealthSync.h"
#include "Medico.h"
#include "Paciente.h"
#include "Perfil.h"
#include "RepGen.h"
#include "ServicoAgenda.h"
#include "ServicoHistorico.h"
#include "ServicoPacientes.h"
#include "Sessao.h"
#include "StatusConsulta.h"
#include "Usuario.h"

// Data futura fixa: o servico rejeita horarios no passado (CA-02.3).
static const DataHora HORARIO_FUTURO(2030, 10, 25, 14, 0);

// -----------------------------------------------------------------------------
// CT-01: Cadastro de paciente com sucesso
// -----------------------------------------------------------------------------
void ct01_cadastroPacienteComSucesso() {
    // DADO QUE a recepcionista esta logada e o repositorio esta vazio
    RepGen repo;
    ServicoPacientes servico(repo);
    Sessao sessaoRecepcionista(Usuario(1, "Recepcionista", Perfil::Recepcionista));

    DadosPaciente dados("Joao Silva", "000.000.000-00");
    Paciente pacienteCriado;

    // QUANDO a recepcionista cadastra um novo paciente
    bool sucesso = servico.cadastrar(dados, pacienteCriado, sessaoRecepcionista);

    // ENTAO o paciente e gravado com sucesso e o CPF confere
    assert(sucesso == true);
    assert(pacienteCriado.getCpf() == "000.000.000-00");
    assert(pacienteCriado.getId() > 0);
    assert(repo.totalPacientes() == 1);
    assert(!servico.getUltimoErro().has_value());
    std::cout << "CT-01 OK\n";
}

// -----------------------------------------------------------------------------
// CT-02: Agendamento sem conflito temporal
// -----------------------------------------------------------------------------
void ct02_agendamentoSemConflito() {
    // DADO QUE paciente, medico e horario sao validos
    RepGen repo;
    ServicoAgenda servicoAgenda(repo);
    Sessao sessaoRecepcionista(Usuario(1, "Recepcionista", Perfil::Recepcionista));

    Paciente paciente(10, "Joao Silva", "000.000.000-00");
    Medico medico(4, "Dr. Marcos");

    // QUANDO a recepcionista agenda uma consulta em horario vago
    std::expected<Consulta, ErroHealthSync> resultado =
        servicoAgenda.agendarConsulta(paciente, medico, HORARIO_FUTURO,
                                      sessaoRecepcionista);

    // ENTAO a consulta e confirmada e o status fica como Agendada
    assert(resultado.has_value());
    assert(resultado->getStatus() == StatusConsulta::Agendada);
    assert(resultado->getStatusStr() == "Agendada");
    assert(resultado->getMedico().getId() == 4);
    assert(resultado->getNomePaciente() == "Joao Silva");
    assert(repo.totalConsultas() == 1);
    std::cout << "CT-02 OK\n";
}

// -----------------------------------------------------------------------------
// CT-03: Concorrencia e conflito (transacao ACID)
// -----------------------------------------------------------------------------
void ct03_concorrenciaEConflito() {
    RepGen repo;
    ServicoAgenda servicoAgenda(repo);
    Sessao sessaoRecepcionista(Usuario(1, "Recepcionista", Perfil::Recepcionista));

    Paciente paciente1(10, "Joao Silva", "000.000.000-00");
    Paciente paciente2(11, "Ana Souza", "111.111.111-11");
    Medico medico(4, "Dr. Marcos");

    // DADO QUE a primeira requisicao ocupa o horario
    auto r1 = servicoAgenda.agendarConsulta(paciente1, medico, HORARIO_FUTURO,
                                            sessaoRecepcionista);
    assert(r1.has_value());

    // QUANDO uma segunda requisicao concorrente tenta o mesmo horario
    auto r2 = servicoAgenda.agendarConsulta(paciente2, medico, HORARIO_FUTURO,
                                            sessaoRecepcionista);

    // ENTAO o sistema impede a gravacao e retorna HorarioOcupado
    assert(!r2.has_value());
    assert(r2.error() == ErroHealthSync::HorarioOcupado);
    assert(repo.totalConsultas() == 1);  // a transacao abortada nao gravou nada

    // E outro medico no mesmo horario continua livre
    Medico outroMedico(5, "Dra. Helena");
    auto r3 = servicoAgenda.agendarConsulta(paciente2, outroMedico, HORARIO_FUTURO,
                                            sessaoRecepcionista);
    assert(r3.has_value());
    assert(repo.totalConsultas() == 2);
    std::cout << "CT-03 OK\n";
}

// -----------------------------------------------------------------------------
// CT-04: Garantia de sigilo clinico
// -----------------------------------------------------------------------------
void ct04_garantiaDeSigiloClinico() {
    RepGen repo;
    ServicoAgenda servicoAgenda(repo);
    ServicoHistorico servicoHistorico(repo);

    Usuario recepUser(1, "Recepcionista", Perfil::Recepcionista);
    Usuario adminUser(2, "Administrador", Perfil::Administrador);
    Medico medicoUser(4, "Dr. Marcos");

    // O registro clinico pertence a uma consulta (composicao): ela precisa existir.
    auto consulta = servicoAgenda.agendarConsulta(
        Paciente(10, "Joao Silva", "000.000.000-00"), medicoUser, HORARIO_FUTURO,
        Sessao(recepUser));
    assert(consulta.has_value());
    int idConsulta = consulta->getId();

    // DADO QUE o medico registra o diagnostico e a prescricao
    bool diagOk = servicoHistorico.registrarDiagnostico(
        idConsulta, "Sintomas gripais leves.", Sessao(medicoUser));
    bool presOk = servicoHistorico.registrarPrescricao(
        idConsulta, "Repouso e hidratacao.", Sessao(medicoUser));
    assert(diagOk && presOk);

    // QUANDO um usuario nao autorizado (recepcionista ou administrador) tenta ler
    auto leituraRecep = servicoHistorico.consultarDiagnostico(idConsulta, Sessao(recepUser));
    auto leituraAdmin = servicoHistorico.consultarPrescricao(idConsulta, Sessao(adminUser));

    // ENTAO o sistema bloqueia e retorna AcessoNegado
    assert(!leituraRecep.has_value());
    assert(leituraRecep.error() == ErroHealthSync::AcessoNegado);
    assert(!leituraAdmin.has_value());
    assert(leituraAdmin.error() == ErroHealthSync::AcessoNegado);

    // E a escrita por nao-medico tambem e barrada
    bool escritaIndevida = servicoHistorico.registrarDiagnostico(
        idConsulta, "Adulterado.", Sessao(recepUser));
    assert(!escritaIndevida);
    assert(servicoHistorico.getUltimoErro() == ErroHealthSync::AcessoNegado);

    // E o medico vinculado continua lendo o dado intacto (CA-04.1)
    auto leituraMedico = servicoHistorico.consultarDiagnostico(idConsulta, Sessao(medicoUser));
    assert(leituraMedico.has_value());
    assert(*leituraMedico == "Sintomas gripais leves.");
    std::cout << "CT-04 OK\n";
}
