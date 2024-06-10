#ifndef AVION
#define AVION

#include <string>

class Avion{
public:
    std::string vuelo;
    std::string numero_de_registro;
    std::string modelo;
    std::string fabricante;
    std::string ano_fabricante;
    std::string capacidad;
    std::string peso_max_despegue;
    std::string aerolinea;
    std::string estado;

    Avion(std::string vuelo, std::string numero_de_registro, std::string modelo, std::string fabricante, std::string ano_fabricante, std::string capacidad, std::string peso_max_despegue, std::string aerolinea, std::string estado)
    : vuelo(vuelo), numero_de_registro(numero_de_registro), modelo(modelo), fabricante(fabricante), ano_fabricante(ano_fabricante), capacidad(capacidad), peso_max_despegue(peso_max_despegue), aerolinea(aerolinea), estado(estado) {};
};

#endif