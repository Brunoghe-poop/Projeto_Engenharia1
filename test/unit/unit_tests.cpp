#undef NDEBUG
#include <cassert>
#include <iostream>
#include <stdexcept>

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
#include "unit_tests.h"

static const DataHora FUTURO(2030, 10, 25, 14, 0);
static const Sessao RECEPCAO(Usuario(1, "Recepcionista", Perfil::Recepcionista));

void ut_dataHora() {
    assert(!DataHora().ehValida());
    assert(DataHora(2028, 2, 29, 10, 0).ehValida());   // bissexto
    assert(!DataHora(2027, 2, 29, 10, 0).ehValida());  // nao bissexto
    assert(!DataHora(2030, 13, 1, 10, 0).ehValida());
    assert(!DataHora(2030, 5, 10, 24, 0).ehValida());

    assert(DataHora(2000, 1, 1, 0, 0).ehPassado());
    assert(!FUTURO.ehPassado());

    assert(FUTURO.str() == "2030-10-25 14:00");
    assert(DataHora("2030-10-25 14:00") == FUTURO);
    assert(!DataHora("lixo").ehValida());

    assert(DataHora(2030, 10, 25, 9, 0) < FUTURO);
    assert(DataHora(2030, 10, 25, 9, 0).mesmoDia(FUTURO));

    DataHora copia(FUTURO);       // construtor de copia
    DataHora atrib;
    atrib = FUTURO;               // atribuicao
    assert(copia == FUTURO && atrib == FUTURO);
    std::cout << "UT dataHora OK\n";
}

void ut_usuarioEMedicoCanonicos() {
    Medico m(4, "Dr. Marcos", "Seg-Sex 8h-12h");
    assert(m.getPerfil() == Perfil::Medico);

    Medico copia(m);
    Medico atrib;
    atrib = m;
    assert(copia.getId() == 4 && copia.getExpediente() == "Seg-Sex 8h-12h");
    assert(atrib.getNome() == "Dr. Marcos" && atrib.getPerfil() == Perfil::Medico);

    Usuario padrao;  // construtor padrao: menor privilegio
    assert(padrao.getId() == 0 && padrao.getPerfil() == Perfil::Paciente);

    Usuario* poli = new Medico(7, "Dra. Helena");  // destrutor virtual
    assert(poli->getPerfil() == Perfil::Medico);
    delete poli;
    std::cout << "UT usuario/medico OK\n";
}

void ut_pacienteCopiaProfunda() {
    Paciente original(1, "Ana", "111.111.111-11");
    original.setAcesso(Usuario(9, "ana", Perfil::Paciente, "ana@x.com"));

    Paciente copia(original);
    Paciente atrib;
    atrib = original;
    assert(copia.getAcesso() != nullptr && atrib.getAcesso() != nullptr);
    assert(copia.getAcesso() != original.getAcesso());  // enderecos distintos
    assert(atrib.getAcesso() != original.getAcesso());
    assert(copia.getAcesso()->getEmail() == "ana@x.com");

    original.removerAcesso();  // nao afeta as copias
    assert(original.getAcesso() == nullptr);
    assert(copia.getAcesso() != nullptr);

    Paciente& ref = atrib;
    atrib = ref;  // auto-atribuicao segura
    assert(atrib.getAcesso() != nullptr);
    std::cout << "UT paciente copia profunda OK\n";
}

void ut_consultaCopiaProfunda() {
    Consulta original(Paciente(1, "Ana", "111.111.111-11"), Medico(4, "Dr. Marcos"), FUTURO);
    assert(original.getRegistro() == nullptr);
    original.obterOuCriarRegistro().setDiagnostico("Gripe");

    Consulta copia(original);
    Consulta atrib;
    atrib = original;
    assert(copia.getRegistro() != original.getRegistro());
    assert(atrib.getRegistro() != original.getRegistro());

    original.obterOuCriarRegistro().setDiagnostico("Alterado");
    assert(copia.getRegistro()->getDiagnostico() == "Gripe");
    assert(atrib.getRegistro()->getDiagnostico() == "Gripe");

    atrib = Consulta();  // sobrescreve com consulta sem registro
    assert(atrib.getRegistro() == nullptr);
    std::cout << "UT consulta copia profunda OK\n";
}

void ut_repGenCopia() {
    RepGen repo;
    ServicoPacientes servico(repo);
    Paciente p;
    assert(servico.cadastrar(DadosPaciente("Ana", "111.111.111-11"), p, RECEPCAO));

    RepGen copia(repo);
    RepGen atrib;
    atrib = repo;
    assert(copia.totalPacientes() == 1 && atrib.totalPacientes() == 1);

    ServicoPacientes servicoCopia(copia);
    Paciente outro;
    assert(servicoCopia.cadastrar(DadosPaciente("Bia", "222.222.222-22"), outro, RECEPCAO));
    assert(copia.totalPacientes() == 2 && repo.totalPacientes() == 1);  // independentes
    std::cout << "UT RepGen copia OK\n";
}

void ut_cadastroRegrasDeNegocio() {
    RepGen repo;
    ServicoPacientes servico(repo);
    Paciente p;

    assert(servico.cadastrar(DadosPaciente("Joao", "000.000.000-00"), p, RECEPCAO));

    // CA-01.2: CPF duplicado (mesmo com formatacao diferente)
    assert(!servico.cadastrar(DadosPaciente("Outro", "00000000000"), p, RECEPCAO));
    assert(servico.getUltimoErro() == ErroHealthSync::CpfDuplicado);
    assert(repo.totalPacientes() == 1);

    // CA-01.3: medico nao pode cadastrar
    Sessao medico(Medico(4, "Dr. Marcos"));
    assert(!servico.cadastrar(DadosPaciente("Maria", "333.333.333-33"), p, medico));
    assert(servico.getUltimoErro() == ErroHealthSync::AcessoNegado);
    assert(repo.totalPacientes() == 1);

    // dados invalidos
    assert(!servico.cadastrar(DadosPaciente("", "333.333.333-33"), p, RECEPCAO));
    assert(!servico.cadastrar(DadosPaciente("Maria", "123"), p, RECEPCAO));
    assert(servico.getUltimoErro() == ErroHealthSync::DadosInvalidos);

    // servico sem repositorio: falha de infraestrutura -> excecao
    ServicoPacientes semRepo;
    bool lancou = false;
    try { semRepo.cadastrar(DadosPaciente("Z", "444.444.444-44"), p, RECEPCAO); }
    catch (const std::runtime_error&) { lancou = true; }
    assert(lancou);
    std::cout << "UT cadastro OK\n";
}

void ut_agendamentoValidacoes() {
    RepGen repo;
    ServicoAgenda agenda(repo);
    Paciente pac(10, "Joao", "000.000.000-00");
    Medico med(4, "Dr. Marcos");

    auto passado = agenda.agendarConsulta(pac, med, DataHora(2020, 1, 1, 8, 0), RECEPCAO);
    assert(!passado && passado.error() == ErroHealthSync::DadosInvalidos);

    auto invalida = agenda.agendarConsulta(pac, med, DataHora(), RECEPCAO);
    assert(!invalida && invalida.error() == ErroHealthSync::DadosInvalidos);

    auto semPaciente = agenda.agendarConsulta(Paciente(), med, FUTURO, RECEPCAO);
    assert(!semPaciente && semPaciente.error() == ErroHealthSync::DadosInvalidos);

    auto negado = agenda.agendarConsulta(pac, med, FUTURO, Sessao(med));
    assert(!negado && negado.error() == ErroHealthSync::AcessoNegado);

    assert(repo.totalConsultas() == 0);  // nada foi gravado
    std::cout << "UT agendamento OK\n";
}

void ut_agendaOcultaCanceladas() {
    RepGen repo;
    ServicoAgenda agenda(repo);
    Paciente pac(10, "Joao", "000.000.000-00");
    Medico med(4, "Dr. Marcos");

    auto tarde = agenda.agendarConsulta(pac, med, DataHora(2030, 10, 25, 15, 0), RECEPCAO);
    auto cedo = agenda.agendarConsulta(pac, med, DataHora(2030, 10, 25, 9, 0), RECEPCAO);
    auto cancelada = agenda.agendarConsulta(pac, med, DataHora(2030, 10, 25, 11, 0), RECEPCAO);
    assert(tarde && cedo && cancelada);

    Consulta c = *repo.buscarConsulta(cancelada->getId());
    c.setStatus(StatusConsulta::Cancelada);
    assert(repo.atualizarConsulta(c));

    auto lista = agenda.consultarAgenda(DataHora(2030, 10, 25, 0, 0), RECEPCAO);
    assert(lista.has_value() && lista->size() == 2);          // CA-03.2
    assert((*lista)[0].getHorario().getHora() == 9);          // ordenada
    assert((*lista)[1].getHorario().getHora() == 15);

    auto negado = agenda.consultarAgenda(FUTURO, Sessao(med));  // CA-03.1: so administrativos
    assert(!negado && negado.error() == ErroHealthSync::AcessoNegado);

    // horario cancelado volta a ficar livre
    auto reagendada = agenda.agendarConsulta(pac, med, DataHora(2030, 10, 25, 11, 0), RECEPCAO);
    assert(reagendada.has_value());
    std::cout << "UT agenda OK\n";
}

void ut_buscaEListagemDePacientes() {
    RepGen repo;
    ServicoPacientes servico(repo);
    Paciente p1, p2;
    assert(servico.cadastrar(DadosPaciente("Ana", "111.111.111-11"), p1, RECEPCAO));
    assert(servico.cadastrar(DadosPaciente("Bia", "222.222.222-22"), p2, RECEPCAO));
    assert(p1.getId() != p2.getId());

    auto achado = servico.buscarPorId(p2.getId(), RECEPCAO);
    assert(achado.has_value() && achado->getNome() == "Bia");

    auto ausente = servico.buscarPorId(999, RECEPCAO);
    assert(!ausente && ausente.error() == ErroHealthSync::NaoEncontrado);

    auto lista = servico.listar(RECEPCAO);
    assert(lista.has_value() && lista->size() == 2);

    auto negado = servico.listar(Sessao(Medico(4, "Dr. Marcos")));
    assert(!negado && negado.error() == ErroHealthSync::AcessoNegado);
    std::cout << "UT busca/listagem de pacientes OK\n";
}

void ut_agendaIndividualDoMedico() {
    RepGen repo;
    ServicoAgenda agenda(repo);
    Paciente pac(10, "Joao", "000.000.000-00");
    Medico marcos(4, "Dr. Marcos"), helena(5, "Dra. Helena");

    assert(agenda.agendarConsulta(pac, marcos, DataHora(2030, 10, 25, 9, 0), RECEPCAO));
    assert(agenda.agendarConsulta(pac, helena, DataHora(2030, 10, 25, 10, 0), RECEPCAO));
    assert(agenda.agendarConsulta(pac, marcos, DataHora(2030, 10, 26, 9, 0), RECEPCAO));

    auto minha = agenda.consultarAgendaDoMedico(DataHora(2030, 10, 25, 0, 0), Sessao(marcos));
    assert(minha.has_value() && minha->size() == 1);              // so as dele, so do dia
    assert((*minha)[0].getMedico().getId() == 4);

    auto negado = agenda.consultarAgendaDoMedico(FUTURO, RECEPCAO);
    assert(!negado && negado.error() == ErroHealthSync::AcessoNegado);
    std::cout << "UT agenda individual do medico OK\n";
}
