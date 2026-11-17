#ifndef MEDICO_H
#define MEDICO_H

#include "Usuario.h"
#include <string>

class Medico : public Usuario {
public:
    Medico();
    Medico(int id, const std::string& nome, const std::string& expediente = "");
    Medico(const Medico& outro);
    Medico& operator=(const Medico& outro);
    ~Medico() override;

    const std::string& getExpediente() const;

private:
    std::string expediente_;
};

#endif  // MEDICO_H
