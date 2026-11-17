#ifndef CONSULTA_H
#define CONSULTA_H

#include "DataHora.h"
#include "Medico.h"
#include "Paciente.h"
#include "RegistroClinico.h"
#include "StatusConsulta.h"
#include <string>

class Consulta {
private:
    int id_;
    DataHora dataHora_;
    int duracaoMin_;
    StatusConsulta status_;
    Paciente paciente_;
    Medico medico_;
    RegistroClinico* registro_;

public:
    //construtor 1
    Consulta();
    
    //construtor 2
    Consulta(const Paciente& paciente, const Medico& medico,
             const DataHora& dataHora, int duracaoMin = 30);
    
    //construtor 3
    Consulta(const Consulta& outra);             
    Consulta& operator=(const Consulta& outra);  
    
    //destrutor
    ~Consulta();
    
    // getters
    int getId() const;
    const DataHora& getDataHora() const;
    const DataHora& getHorario() const;  // sinonimo de getDataHora()
    int getDuracaoMin() const;
    StatusConsulta getStatus() const;
    std::string getStatusStr() const;
    const Medico& getMedico() const;
    const Paciente& getPaciente() const;
    const std::string& getNomePaciente() const;
    const RegistroClinico* getRegistro() const;  // nullptr se ainda nao existe
    RegistroClinico& obterOuCriarRegistro();     // cria sob demanda (composicao)

    // setters
    void setId(int id);
    void setStatus(StatusConsulta status);
};

#endif  // CONSULTA_H
