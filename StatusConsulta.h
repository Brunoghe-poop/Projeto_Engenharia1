#ifndef STATUSCONSULTA_H
#define STATUSCONSULTA_H

#include <string>

enum class StatusConsulta { Agendada, Confirmada, Cancelada };

std::string paraString(StatusConsulta status);

#endif  // STATUSCONSULTA_H
