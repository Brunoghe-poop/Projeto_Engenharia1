#ifndef DADOSPACIENTE_H
#define DADOSPACIENTE_H

#include <string>

// Dados de entrada do cadastro (objeto de transferencia, sem id)
class DadosPaciente {
private:
    std::string nome_;
    std::string cpf_;
    std::string telefone_;    
public:
    // construtor 1
    DadosPaciente();

    // construtor 2
    DadosPaciente(const std::string& nome, const std::string& cpf,
                  const std::string& telefone = "");
    
    // construtor 3
    DadosPaciente(const DadosPaciente& outros);
    DadosPaciente& operator=(const DadosPaciente& outros);

    // destrutor
    ~DadosPaciente();
    
    //getters
    const std::string& getNome() const;
    const std::string& getCpf() const;
    const std::string& getTelefone() const;
};

#endif  // DADOSPACIENTE_H
