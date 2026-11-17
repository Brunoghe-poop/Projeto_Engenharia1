#ifndef REGISTROCLINICO_H
#define REGISTROCLINICO_H

#include <string>

class RegistroClinico {
public:
    RegistroClinico();
    RegistroClinico(const RegistroClinico& outro);
    RegistroClinico& operator=(const RegistroClinico& outro);
    ~RegistroClinico();

    void setDiagnostico(const std::string& texto);
    void setPrescricao(const std::string& texto);
    const std::string& getDiagnostico() const;
    const std::string& getPrescricao() const;

private:
    std::string diagnostico_;
    std::string prescricao_;
};

#endif  // REGISTROCLINICO_H
