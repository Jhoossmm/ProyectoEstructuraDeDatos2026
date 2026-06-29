#ifndef METRICAS_H
#define METRICAS_H

#include <unordered_map>
#include <string>
#include "Grafo.h"

using namespace std;

class Metricas {
public:
    // calcula degree centrality usando vecinos incidentes
    static unordered_map<int, double> degree_centrality(const Grafo& grafo);

    // imprime el top k de nodos por degree centrality
    static void print_degree(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k = 5);

    // calcula pagerank iterativo usando aristas salientes
    static unordered_map<int, double> pagerank(
        const Grafo& grafo,
        double damping = 0.85,
        int max_iter = 100,
        double tolerancia = 1e-6,
        int k_nodos = -1
    );

    // imprime el top k de nodos por pagerank
    static void print_pagerank(
        const Grafo& grafo,
        const unordered_map<int, string>& id_a_nombre,
        int k = 5,
        double damping = 0.85,
        int max_iter = 100,
        double tolerancia = 1e-6,
        int k_nodos = -1
    );

    // calcula betweenness centrality con brandes
    // usa bfs si no hay pesos y dijkstra si hay pesos
    // k_origenes = -1 usa todos los nodos de origen 
    // k_origenes > 0 usa solo k nodos para un aproximado 
    static unordered_map<int, double> betweenness_centrality(const Grafo& grafo, int k_origenes = -1);

    // imprime el top k de nodos por betweenness
    static void print_betweenness(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k = 5, int k_origenes = -1);

    //Calcula la excentricidad de un vertice especifico
    static double excentricidad(const Grafo& grafo, int origen, bool es_ponderado);

    // Calcula el Diámetro (la mayor excentricidad de todo el grafo)
    static double diametro(const Grafo& grafo, bool es_ponderado);
    
    //Calcula la Radiality Centrality
    static double radiality_centrality(const Grafo& grafo, int origen, bool es_ponderado, double diametro_grafo = -1.0);

    // calcula el shortest path promedio entre pares alcanzables
    // k_origenes = -1 usa todos los nodos como origen
    // k_origenes > 0 usa solo k origenes para aproximar
    static double average_shortest_path(const Grafo& grafo, int k_origenes = -1);

    // imprime average shortest path, exacto o aproximado
    static void print_average_shortest_path(const Grafo& grafo, int k_origenes = -1);

    // calcula closeness centrality 
    // k_origenes = -1 usa todos los nodos como origen
    // k_origenes > 0 usa solo k nodos para aproximar el resultado
    static unordered_map<int, double> closeness_centrality(const Grafo& grafo, int k_origenes = -1);

    // imprime el top k de nodos por closeness.
    static void print_closeness(const Grafo& grafo, const unordered_map<int, string>& id_a_nombre, int k = 5, int k_origenes = -1);

};

#endif