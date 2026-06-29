#include <iostream>
#include "Grafo.h"
#include "Loader.h"
#include "Metricas.h"


using namespace std;

int main() {
    cout << "INICIANDO PRUEBAS DE CARGA...\n\n";

    // --- PRUEBA 1: Dataset IMDb ---
    cout << "=== Test 1: Red IMDb (No Dirigido) ===\n";
    Grafo grafo_imdb; 
    Loader loader_imdb;
    
    loader_imdb.cargar_csv("data/imdb_edgelist.csv", grafo_imdb, ',', false, false, false);
    auto mapa_imdb = loader_imdb.get_mapa_nombres();
    grafo_imdb.imprimir_muestra_nodos(mapa_imdb);

    cout << "\n=== Test 2: Red Kraggle (Dirigido y Ponderado) ===\n";
    
    // --- PRUEBA 2: Dataset Kraggle ---
    Grafo grafo_kraggle; 
    Loader loader_kraggle;
    
    loader_kraggle.cargar_csv("data/edges.csv", grafo_kraggle, ',', true, true, true);
    auto mapa_kraggle = loader_kraggle.get_mapa_nombres();
    grafo_kraggle.imprimir_muestra_nodos(mapa_kraggle);

    // --- PRUEBA 3: Degree Centrality en Dataset IMDb y Kraggle

    auto metricas = calcular_degree_centrality(grafo_imdb);
    imprimir_degree_centrality(metricas, mapa_imdb, 10);

    auto metricas_kaggle = calcular_degree_centrality(grafo_kraggle);
    imprimir_degree_centrality(metricas_kaggle, mapa_kraggle, 10);

    medir_tiempo_degree_centrality(grafo_imdb, 10);
    medir_tiempo_degree_centrality(grafo_kraggle, 10);

    return 0;

}