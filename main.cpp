#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "libs/json.hpp"
#include "ListaCircularDoble.h"
#include "Cola.h"
#include "Pila.h"
#include "Pasajero.h"
#include "Avion.h"

using json = nlohmann::json;

ListaCircularDoble listaDisponibles;
ListaCircularDoble listaMantenimiento;
Cola colaPasajeros;
Pila pilaEquipajes;

void cargarAviones(const std::string& archivo) {
    std::ifstream ifs(archivo);
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << archivo << std::endl;
        return;
    }
    json obj;
    ifs >> obj; 

    for (const auto& avion : obj) {
        Avion* nuevoAvion = new Avion(
            avion["vuelo"].get<std::string>(),
            avion["numero_de_registro"].get<std::string>(),
            avion["modelo"].get<std::string>(),
            avion["fabricante"].get<std::string>(),
            avion["ano_fabricacion"].get<int>(),
            avion["capacidad"].get<int>(),
            avion["peso_max_despegue"].get<int>(),
            avion["aerolinea"].get<std::string>(),
            avion["estado"].get<std::string>()
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
    if (!ifs.is_open()) {
        std::cerr << "Error opening file: " << archivo << std::endl;
        return;
    }
    json obj;
    ifs >> obj;

    for (const auto& pasajero : obj["pasajeros"]) {
        Pasajero* nuevoPasajero = new Pasajero(
            pasajero["nombre"].get<std::string>(),
            pasajero["nacionalidad"].get<std::string>(),
            pasajero["numero_de_pasaporte"].get<std::string>(),
            pasajero["vuelo"].get<std::string>(),
            pasajero["asiento"].get<int>(),
            pasajero["destino"].get<std::string>(),
            pasajero["origen"].get<std::string>(),
            pasajero["equipaje_facturado"].get<int>()
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
        std::string archivo_prueba;

        switch (opcion) {
            case 1:
                std::cout << "Ingrese el nombre del archivo de aviones: ";
                std::cin >> archivo;
                archivo_prueba = "C:\\Proyectos\\U\\EDD\\practica\\archivos_prueba\\aviones.json";
                cargarAviones(archivo_prueba);
                break;
            case 2:
                std::cout << "Ingrese el nombre del archivo de pasajeros: ";
                std::cin >> archivo;
                archivo_prueba = "C:\\Proyectos\\U\\EDD\\practica\\archivos_prueba\\pasajeros.json";
                cargarPasajeros(archivo_prueba);
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
