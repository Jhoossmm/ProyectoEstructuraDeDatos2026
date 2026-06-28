#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

//Estructura para almacenar la relacion completa
struct Arista {
    int destino;
    double peso;
};

class Grafo {
private:
    //Lista de adyacencia: ID Origen -> Vector Arista
    unordered_map<int, vector<Arista>> lista_adyacencia;
    bool es_dirigido;

public:
    Grafo();

    //Función para insertar una arista al grafo
    void insertar_arista(int u, int v, double peso, bool es_dirigida);
    
    //Función de prueba
    void imprimir_muestra_nodos(unordered_map<int, string>& id_a_nombre);
};

#endif