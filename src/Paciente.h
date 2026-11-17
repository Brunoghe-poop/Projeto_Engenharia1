#ifndef PACIENTE_H
#define PACIENTE_H

#include "Usuario.h"
#include <string>

class Paciente {
public:
    Paciente();
    Paciente(int id, const std::string& nome, const std::string& cpf,
             const std::string& telefone = "");
    Paciente(const Paciente& outro);             // copia profunda do acesso
    Paciente& operator=(const Paciente& outro);  // copia profunda do acesso
    ~Paciente();                                 // libera o acesso

    int getId() const;
    const std::string& getNome() const;
    const std::string& getCpf() const;
    const std::string& getTelefone() const;
    const Usuario* getAcesso() const;  // nullptr se nao ha login

    // remove pontuacao: "000.000.000-00" -> "00000000000"
    static std::string normalizarCpf(const std::string& cpf);

    void setId(int id);
    void setAcesso(const Usuario& usuario);
    void removerAcesso();

private:
    int id_;
    std::string nome_;
    std::string cpf_;
    std::string telefone_;
    Usuario* acesso_;
};

#endif  // PACIENTE_H
