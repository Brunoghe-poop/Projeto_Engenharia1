#include <iostream>

#include "unit_tests.h"

int main() {
    std::cout << "=== HealthSync: testes unitarios ===\n";
    ut_dataHora();
    ut_usuarioEMedicoCanonicos();
    ut_pacienteCopiaProfunda();
    ut_consultaCopiaProfunda();
    ut_repGenCopia();
    ut_cadastroRegrasDeNegocio();
    ut_agendamentoValidacoes();
    ut_agendaOcultaCanceladas();
    ut_buscaEListagemDePacientes();
    ut_agendaIndividualDoMedico();
    std::cout << "Todos os testes unitarios passaram.\n";
    return 0;
}
