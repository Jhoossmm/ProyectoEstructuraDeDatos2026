#include "Loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

//El constructor inicializa el contador de IDs
Loader::Loader() : id_actual(0) {}

//Logica para mapear strings a enteros únicos
int Loader::obtener_o_crear_id(const string& nombre) {
    //Si el nombre no se ha leido, crear un nuevo ID, sino retornar el existente
    if (nombre_a_id.find(nombre) == nombre_a_id.end()) {
        nombre_a_id[nombre] = id_actual;
        id_a_nombre[id_actual] = nombre;
        id_actual++;
    }

    return nombre_a_id[nombre];
}

//Funcion para retornar el diccionario inverso
unordered_map<int, string> Loader::get_mapa_nombres() {
    return id_a_nombre;
}

//Funcion principal de lectura
void Loader::cargar_csv(const string& ruta_archivo, Grafo& grafo, char delimitador, bool tiene_peso, bool tiene_relacion, bool es_dirigido_por_defecto) {
    
    ifstream archivo(ruta_archivo);
    if (!archivo.is_open()) {
        cerr << "[!] ERROR: No se pudo abrir el archivo. " << ruta_archivo << "\n";
        return;
    }

    string linea;
    getline(archivo, linea); //Ignorar cabecera

    int lineas_leidas = 0;
    while (getline(archivo, linea)) {
        //Usar stringstream para dividir la línea por el delimitador
        stringstream ss(linea);
        string origen_str, destino_str, relacion_str, peso_str;

        getline(ss, origen_str, delimitador);
        getline(ss, destino_str, delimitador);

        //Si el formato es incorrecto, saltar la línea
        if (origen_str.empty() || destino_str.empty()) continue;
        
        //Obtener o crear IDs para los nodos de origen y destino
        int u = obtener_o_crear_id(origen_str);
        int v = obtener_o_crear_id(destino_str);
        
        //Valores por defecto
        string relacion = "DEFAULT"; 
        double peso = 1.0;
        bool es_dirigida = es_dirigido_por_defecto;

        //Si el dataset tiene columna de relación, leerla y ajustar la dirección según la lógica semántica
        if (tiene_relacion) {
            getline(ss, relacion_str, delimitador);
            relacion = relacion_str;
            
            //Relaciones semanticas especificas de Kaggle
            if (relacion == "TEAMED_WITH") {
                es_dirigida = false;
            } else { // PARTICIPATED o HOSTED
                es_dirigida = true;
            }
        }

        //Si el archivo tiene un campo de peso, leerlo
        if (tiene_peso && getline(ss, peso_str, delimitador)) {
            try {
                peso = stod(peso_str);
            } catch (...) {
                peso = 1.0;
            }
        }

        //Insertar la arista en el grafo
        grafo.insertar_arista(u, v, peso, es_dirigida);
        lineas_leidas++;
    }

    archivo.close();
    cout << "[OK] Se cargaron " << lineas_leidas << " aristas desde '" << ruta_archivo << "'\n";
}