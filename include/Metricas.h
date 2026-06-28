#ifndef METRICAS_H
#define METRICAS_H

#include <unordered_map>
#include <string>
#include "Grafo.h"

using namespace std;

class Metricas {
public:
    // calcula betweenness centrality con brandes
    // usa bfs si no hay pesos y dijkstra si hay pesos
    // k_origenes = -1 usa todos los nodos de origen 
    // k_origenes > 0 usa solo k nodos para un aproximado 
    static unordered_map<int, double> betweenness_centrality(const Grafo& grafo, int k_origenes = -1);

    // imprime el top k de nodos por betweenness
    static void print_betweenness(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k = 5, int k_origenes = -1);

    //Calcula la excentricidad de un vertice especifico
    static double excentricidad(const Grafo& grafo, int origen, bool es_ponderado);
};

#endif