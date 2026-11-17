#ifndef ERROHEALTHSYNC_H
#define ERROHEALTHSYNC_H

#include <string>

enum class ErroHealthSync {
    CpfDuplicado,
    HorarioOcupado,
    AcessoNegado,
    DadosInvalidos,
    NaoEncontrado
};

std::string paraString(ErroHealthSync erro);

#endif  // ERROHEALTHSYNC_H
