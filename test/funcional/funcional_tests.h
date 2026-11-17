// Testes funcionais do HealthSync: um por cenario de teste (CT) do documento.
// Cada funcao monta seu proprio RepGen, portanto sao independentes entre si.
#ifndef FUNCIONAL_TESTS_H
#define FUNCIONAL_TESTS_H

void ct01_cadastroPacienteComSucesso();  // CASO-01 / CA-01.1
void ct02_agendamentoSemConflito();      // CASO-02 / CA-02.1
void ct03_concorrenciaEConflito();       // CASO-02 / CA-02.2 (ACID)
void ct04_garantiaDeSigiloClinico();     // CASO-04 e 05 / CA-04.1 e CA-04.2

#endif  // FUNCIONAL_TESTS_H
