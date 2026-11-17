#ifndef PERFIL_H
#define PERFIL_H

#include <string>

enum class Perfil { Administrador, Gerente, Medico, Recepcionista, Paciente };

std::string paraString(Perfil perfil);

// Recepcionista e os perfis que herdam suas permissoes (gerente, administrador)
bool temPermissaoRecepcao(Perfil perfil);

#endif  // PERFIL_H
