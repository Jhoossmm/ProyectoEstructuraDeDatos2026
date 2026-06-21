#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <unordered_map>
#include "Grafo.h"

using namespace std;

class Loader {
private:
    //Mapeo de nombres a IDs y viceversa
    unordered_map<string, int> nombre_a_id;
    unordered_map<int, string> id_a_nombre;
    int id_actual;

    //Metodo auxiliar
    int obtener_o_crear_id(const string& nombre);

public:
    Loader();

    unordered_map<int, string> get_mapa_nombres();

    void cargar_csv(const string& ruta_archivo, Grafo& grafo, char delimitador, bool tiene_peso, bool tiene_relacion, bool es_dirigido_por_defecto);
};

#endif