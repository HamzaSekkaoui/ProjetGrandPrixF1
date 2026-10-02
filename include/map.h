/**
 * @file map.h
 * @brief Gestion de la carte et du calcul de l'heuristique BFS.
 * @author Hamza SEKKAOUI
 * @author hamza hour
 * @author reda sarsri
 */

#ifndef MAP_H
#define MAP_H

#include "follow_line.h"

#define MAX_LINE_LENGTH 1024
#define INF 999999

/**
 * @brief Structure représentant la carte et la matrice des heuristiques.
 */
typedef struct {
    int width;       /**< Largeur de la grille */
    int height;      /**< Hauteur de la grille */
    int initialGas;  /**< Carburant initial fourni */
    char **grid;     /**< Grille 2D des cases du circuit */
    int **heuristic; /**< Distances pré-calculées vers l'arrivée */
} Map;

extern Map map;

/**
 * @brief Lit la carte depuis l'entrée standard et alloue la mémoire.
 */
void read_map(void);

/**
 * @brief Calcule la distance heuristique (BFS) de chaque case vers l'arrivée.
 */
void compute_heuristic(void);

/**
 * @brief Libère la mémoire allouée dynamiquement pour la carte.
 */
void free_map(void);

#endif /* MAP_H */