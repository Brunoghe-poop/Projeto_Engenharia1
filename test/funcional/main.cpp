// Ponto de entrada dos testes funcionais (testes regressivos do HealthSync).
#include <iostream>

#include "funcional_tests.h"

// Executa todos os cenarios de teste em sequencia. Qualquer assert que falhar
// interrompe o programa, sinalizando regressao.
void testesRegressivos() {
    ct01_cadastroPacienteComSucesso();
    ct02_agendamentoSemConflito();
    ct03_concorrenciaEConflito();
    ct04_garantiaDeSigiloClinico();
}

int main() {
    std::cout << "=== HealthSync: testes funcionais (regressivos) ===\n";
    testesRegressivos();
    std::cout << "Todos os testes funcionais passaram.\n";
    return 0;
}
