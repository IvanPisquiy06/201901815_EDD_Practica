#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h> // Incluye la librería JSON para C++
#include "ListaCircularDoble.h"
#include "Cola.h"
#include "Pila.h"

ListaCircularDoble listaDisponibles;
ListaCircularDoble listaMantenimiento;
Cola colaPasajeros;
Pila pilaEquipajes;

void cargarAviones(const std::string& archivo) {
    std::ifstream ifs(archivo);
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    
    for (auto& avion : obj["aviones"]) {
        Avion* nuevoAvion = new Avion(
            avion["codigo"].asString(),
            avion["modelo"].asString(),
            avion["estado"].asString()
        );
        if (nuevoAvion->estado == "Disponible") {
            listaDisponibles.insertar(nuevoAvion);
        } else if (nuevoAvion->estado == "Mantenimiento") {
            listaMantenimiento.insertar(nuevoAvion);
        }
    }
}

void cargarPasajeros(const std::string& archivo) {
    std::ifstream ifs(archivo);
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj);
    
    for (auto& pasajero : obj["pasajeros"]) {
        Pasajero* nuevoPasajero = new Pasajero(
            pasajero["nombre"].asString(),
            pasajero["pasaporte"].asString(),
            pasajero["vuelo"].asString(),
            pasajero["asiento"].asInt()
        );
        colaPasajeros.encolar(nuevoPasajero);
    }
}

void mostrarMenu() {
    std::cout << "1. Cargar aviones" << std::endl;
    std::cout << "2. Cargar pasajeros" << std::endl;
    std::cout << "3. Mostrar aviones disponibles" << std::endl;
    std::cout << "4. Mostrar aviones en mantenimiento" << std::endl;
    std::cout << "5. Registrar equipajes" << std::endl;
    std::cout << "6. Salir" << std::endl;
}

int main() {
    int opcion;
    std::string archivo;

    while (true) {
        mostrarMenu();
        std::cin >> opcion;

        switch (opcion) {
            case 1:
                std::cout << "Ingrese el nombre del archivo de aviones: ";
                std::cin >> archivo;
                cargarAviones(archivo);
                break;
            case 2:
                std::cout << "Ingrese el nombre del archivo de pasajeros: ";
                std::cin >> archivo;
                cargarPasajeros(archivo);
                break;
            case 3:
                std::cout << "Aviones disponibles:" << std::endl;
                listaDisponibles.mostrar();
                break;
            case 4:
                std::cout << "Aviones en mantenimiento:" << std::endl;
                listaMantenimiento.mostrar();
                break;
            case 5:
                while (!colaPasajeros.estaVacia()) {
                    Pasajero* pasajero = colaPasajeros.desencolar();
                    if (pasajero) {
                        pilaEquipajes.apilar(pasajero);
                        std::cout << "Equipaje registrado para pasajero: " << pasajero->nombre << std::endl;
                    }
                }
                break;
            case 6:
                return 0;
            default:
                std::cout << "Opción no válida." << std::endl;
        }
    }

    return 0;
}
