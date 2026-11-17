#ifndef USUARIO_H
#define USUARIO_H

#include "Perfil.h"
#include <string>

class Usuario {
public:
    Usuario();
    Usuario(int id, const std::string& nome, Perfil perfil,
            const std::string& email = "");
    Usuario(const Usuario& outro);
    Usuario& operator=(const Usuario& outro);
    virtual ~Usuario();  // virtual: Medico e destruido corretamente via Usuario*

    int getId() const;
    const std::string& getNome() const;
    const std::string& getEmail() const;
    Perfil getPerfil() const;

private:
    int id_;
    std::string nome_;
    std::string email_;
    Perfil perfil_;
};

#endif  // USUARIO_H
