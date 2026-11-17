#ifndef SESSAO_H
#define SESSAO_H

#include "Perfil.h"
#include "Usuario.h"

class Sessao {
public:
    Sessao();
    explicit Sessao(const Usuario& usuario);
    Sessao(const Sessao& outra);
    Sessao& operator=(const Sessao& outra);
    ~Sessao();

    const Usuario& getUsuario() const;
    int getIdUsuario() const;
    Perfil getPerfil() const;

private:
    Usuario usuario_;
};

#endif  // SESSAO_H
