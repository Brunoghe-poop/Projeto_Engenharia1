#include "ErroHealthSync.h"

std::string paraString(ErroHealthSync erro) {
    switch (erro) {
        case ErroHealthSync::CpfDuplicado:   return "CpfDuplicado";
        case ErroHealthSync::HorarioOcupado: return "HorarioOcupado";
        case ErroHealthSync::AcessoNegado:   return "AcessoNegado";
        case ErroHealthSync::DadosInvalidos: return "DadosInvalidos";
        case ErroHealthSync::NaoEncontrado:  return "NaoEncontrado";
    }
    return "Desconhecido";
}
