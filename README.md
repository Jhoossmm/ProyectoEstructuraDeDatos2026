# Proyecto Semestral: Métricas de Centralidad en Redes

**Universidad de Concepción**

**Facultad de Ingeniería - Ingeniería Civil Informática**

**Asignatura:** Estructura de Datos (S1-2026) 

**Docente:** José Sebastián Fuentes Sepúlveda 

**Grupo:** 15 

**Integrantes:**
* Felipe Alejandro Tilleria Morales 
* Jhostian Matías San Martín Morales 
* Matías Ignacio Catril Pérez

**Compilación y Ejecución:**

  g++ src/main.cpp src/Grafo.cpp src/Loader.cpp src/Metricas.cpp src/Algoritmos.cpp -I include -o proyecto -std=c++14

  En Linux/macOS: ./proyecto
  
  En Windows: proyecto.exe o ./proyecto 
  
---

## Descripción del Proyecto

Se implementa un TDA Grafo en C++ enfocado en el análisis de redes complejas.
El objetivo principal es cargar datasets que representan redes reales y calcular distintas métricas de centralidad para identificar los nodos más importantes dentro de la topología.

El TDA está diseñado de manera genérica, soportando grafos dirigidos, no dirigidos, ponderados y heterogéneos (múltiples tipos de relaciones).

## Datasets usados

- IMDb Actors Network: Red no dirigida y ponderada.
- Kaggle Competition Graph: Grafo heterogéneo dirigido/no dirigido y ponderado.

## Checklist de Métricas implementadas

- [ ] Degree Centrality:

- [ ] Betweenness Centrality:

- [ ] Closeness Centrality:

- [ ] PageRank:

- [ ] Average Shortest Path:


- [ ] Eccentricity:

- [ ] Radiality Centrality:
