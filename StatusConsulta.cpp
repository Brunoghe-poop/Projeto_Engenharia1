#include "StatusConsulta.h"

std::string paraString(StatusConsulta status) {
    switch (status) {
        case StatusConsulta::Agendada:   return "Agendada";
        case StatusConsulta::Confirmada: return "Confirmada";
        case StatusConsulta::Cancelada:  return "Cancelada";
    }
    return "Desconhecido";
}
