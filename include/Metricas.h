#ifndef METRICAS_H
#define METRICAS_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Grafo.h"

using namespace std;

struct MetricasGrado {
    int    id;
    int    grado_entrada;
    int    grado_salida;
    int    grado_total;
    double centralidad_entrada;
    double centralidad_salida;
    double centralidad_total;
};

// Calcula degree centrality para todos los nodos y retorna un vector ordenado de mayor a menor grado total
vector<MetricasGrado> calcular_degree_centrality(const Grafo& grafo);

// Imprime tabla con los n nodos de mayor centralidad
void imprimir_degree_centrality(
    const vector<MetricasGrado>& metricas,
    unordered_map<int, string>& id_a_nombre,
    int top_n = 10
);

void medir_tiempo_degree_centrality(const Grafo& grafo, int repeticiones = 10);

#endif