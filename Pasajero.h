#ifndef PASAJERO_H
#define PASAJERO_H

#include <string>

class Pasajero {
public:
    std::string nombre;
    std::string nacionalidad;
    std::string numero_de_pasaporte;
    std::string vuelo;
    int asiento;
    std::string destino;
    std::string origen;
    int equipaje_facturado;


    Pasajero(std::string nombre, std::string nacionalidad, std::string numero_de_pasaporte, 
        std::string vuelo, int asiento, std::string destino, std::string origen, int equipaje_facturado)
    : nombre(nombre), nacionalidad(nacionalidad), numero_de_pasaporte(numero_de_pasaporte), 
        vuelo(vuelo), asiento(asiento), destino(destino), origen(origen), equipaje_facturado(equipaje_facturado) {};
};

#endif // PASAJERO_H
