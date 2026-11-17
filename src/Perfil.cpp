#include "Perfil.h"

std::string paraString(Perfil perfil) {
    switch (perfil) {
        case Perfil::Administrador: return "Administrador";
        case Perfil::Gerente:       return "Gerente";
        case Perfil::Medico:        return "Medico";
        case Perfil::Recepcionista: return "Recepcionista";
        case Perfil::Paciente:      return "Paciente";
    }
    return "Desconhecido";
}

bool temPermissaoRecepcao(Perfil perfil) {
    return perfil == Perfil::Recepcionista || perfil == Perfil::Gerente ||
           perfil == Perfil::Administrador;
}
