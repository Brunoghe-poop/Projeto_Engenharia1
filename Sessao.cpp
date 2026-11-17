#include "Sessao.h"

Sessao::Sessao() : usuario_() {}
Sessao::Sessao(const Usuario& usuario) : usuario_(usuario) {}
Sessao::Sessao(const Sessao& outra) : usuario_(outra.usuario_) {}

Sessao& Sessao::operator=(const Sessao& outra) {
    if (this != &outra) usuario_ = outra.usuario_;
    return *this;
}

Sessao::~Sessao() {}

const Usuario& Sessao::getUsuario() const { return usuario_; }
int Sessao::getIdUsuario() const { return usuario_.getId(); }
Perfil Sessao::getPerfil() const { return usuario_.getPerfil(); }
