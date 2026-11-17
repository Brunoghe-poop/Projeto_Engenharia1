// Testes unitarios: verificam a FORMA CANONICA de cada classe concreta e as
// regras isoladas de validacao/permissao.
#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

void ut_dataHora();                      // validade, calendario, ordenacao, texto
void ut_usuarioEMedicoCanonicos();       // copia, atribuicao e heranca
void ut_pacienteCopiaProfunda();         // Usuario* acesso_ nao e compartilhado
void ut_consultaCopiaProfunda();         // RegistroClinico* nao e compartilhado
void ut_repGenCopia();                   // copia independente do original
void ut_cadastroRegrasDeNegocio();       // CA-01.2, CA-01.3 e dados invalidos
void ut_agendamentoValidacoes();         // CA-02.3 e permissao
void ut_agendaOcultaCanceladas();        // CA-03.1 e CA-03.2

void ut_buscaEListagemDePacientes();
void ut_agendaIndividualDoMedico();   // so o proprio medico, so o dia pedido

#endif  // UNIT_TESTS_H
