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

    // betweenness centrality con 5000 nodos para aproximar  
    // Metricas::print_betweenness(grafo_imdb, mapa_imdb, 5, 5000);

    // -------------------------------------------------------------
    // Test combinado de Hub, Excentricidad y Radialidad)
    // -------------------------------------------------------------
    if (!grafo_imdb.get_nodos().empty()) {
        int id_hub = -1;
        size_t max_conexiones = 0;
        
        //Buscar al actor con más conexiones
        for (const auto& par : grafo_imdb.get_lista_adyacencia()) {
            if (par.second.size() > max_conexiones) {
                max_conexiones = par.second.size();
                id_hub = par.first;
            }
        }

        string nombre_actor = "Desconocido";
        if (mapa_imdb.find(id_hub) != mapa_imdb.end()) {
            nombre_actor = mapa_imdb[id_hub];
        }
        
        //Calcular las métricas locales
        double exc_imdb = Metricas::excentricidad(grafo_imdb, id_hub, false);
        
        // Le pasamos un diámetro constante de 15 basado en la teoría de Small-World
        double rad_imdb = Metricas::radiality_centrality(grafo_imdb, id_hub, false, 15.0);
        
        cout << "-> El Hub principal de IMDb es [" << id_hub << "] (" << nombre_actor << ") con " 
        << max_conexiones << " conexiones.\n";
        cout << "-> La Excentricidad del Hub es: " << exc_imdb << "\n";
        cout << "-> La Radiality Centrality del Hub es: " << rad_imdb << "\n";

        // // shortest path promedio con 2000 nodos para aproximar
        // Metricas::print_average_shortest_path(grafo_imdb, 2000);

        // closeness centrality top 5 aproximado con 1000 nodos
        Metricas::print_closeness(grafo_imdb, mapa_imdb, 5, 1000);
    }
    // -------------------------------------------------------------


    cout << "\n=== Test 2: Red Kraggle (Dirigido y Ponderado) ===\n";
    
    // --- PRUEBA 2: Dataset Kraggle ---
    Grafo grafo_kraggle; 
    Loader loader_kraggle;
    
    loader_kraggle.cargar_csv("data/edges.csv", grafo_kraggle, ',', true, true, true);
    auto mapa_kraggle = loader_kraggle.get_mapa_nombres();
    grafo_kraggle.imprimir_muestra_nodos(mapa_kraggle);
    
    // betweenness centrality con 5000 nodos para aproximar
    // Metricas::print_betweenness(grafo_kraggle, mapa_kraggle, 5, 5000);

    // -------------------------------------------------------------
    // Test combinado de Hub, Excentricidad y Radialidad)
    // -------------------------------------------------------------
    if (!grafo_kraggle.get_nodos().empty()) {
        int id_hub = -1;
        size_t max_conexiones = 0;
        
        // 1. Buscar al actor con más conexiones
        for (const auto& par : grafo_kraggle.get_lista_adyacencia()) {
            if (par.second.size() > max_conexiones) {
                max_conexiones = par.second.size();
                id_hub = par.first;
            }
        }

        string nombre_actor = "Desconocido";
        if (mapa_kraggle.find(id_hub) != mapa_kraggle.end()) {
            nombre_actor = mapa_kraggle[id_hub];
        }
        
        //Calcular las métricas locales
        double exc_kraggle = Metricas::excentricidad(grafo_kraggle, id_hub, true);
        
        // Le pasamos un diámetro constante de 15 basado en la teoría de Small-World
        double rad_kraggle = Metricas::radiality_centrality(grafo_kraggle, id_hub, true, 15.0);
        
        cout << "-> El Hub principal de Kraggle es [" << id_hub << "] (" << nombre_actor << ") con " 
        << max_conexiones << " conexiones.\n";
        cout << "-> La Excentricidad del Hub es: " << exc_kraggle << "\n";
        cout << "-> La Radiality Centrality del Hub es: " << rad_kraggle << "\n";
       
        // // shortest path promedio con 2000 nodos para aproximar
        // Metricas::print_average_shortest_path(grafo_kraggle, 2000);
    
        // closeness centrality top 5 aproximado con 1000 nodos
        Metricas::print_closeness(grafo_kraggle, mapa_kraggle, 5, 1000);
    }
    // -------------------------------------------------------------
    return 0;
}