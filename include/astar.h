/**
 * @file astar.h
 * @brief Définitions pour l'algorithme A* et la file de priorité.
 * @author Hamza SEKKAOUI
 * @author hamza hour
 * @author reda sarsri
 */

#ifndef ASTAR_H
#define ASTAR_H

#include "map.h"
#include "follow_line.h"

#define MAX_NODES 400000

/**
 * @brief Nœud utilisé par l'algorithme A* pour explorer les déplacements.
 */
typedef struct {
    int f, g, h;            /**< Coûts A* : f = g + h */
    int x, y, vx, vy;       /**< État cinématique de la voiture */
    int gas_left;           /**< Carburant restant */
    int gas_spent;          /**< Carburant consommé depuis le départ */
    int boosts_left;        /**< Nombre de boosts disponibles */
    int first_ax, first_ay; /**< Accélération initiale conduisant à cet état */
} Node;

extern Pos2Dint me, adv1, adv2;
extern int current_vx, current_vy;
extern int my_boosts;
extern int my_gas;

/**
 * @brief Alloue les structures internes pour la recherche A* (tas binaire, tags de visite).
 */
void init_astar(void);

/**
 * @brief Libère la mémoire réservée par l'algorithme A*.
 */
void free_astar(void);

/**
 * @brief Vérifie la validité d'un déplacement (trajectoire sans collision).
 * @param start_x Coordonnée X de départ.
 * @param start_y Coordonnée Y de départ.
 * @param end_x Coordonnée X d'arrivée.
 * @param end_y Coordonnée Y d'arrivée.
 * @param speed_sq Vitesse au carré lors du déplacement.
 * @param is_sand Pointeur modifié (1 si la trajectoire passe sur du sable, 0 sinon).
 * @param g_depth Profondeur de la recherche (utilisée pour esquiver les adversaires au 1er tour).
 * @return 1 si le mouvement est valide, 0 s'il y a collision ou infraction.
 */
int is_valid_move(int start_x, int start_y, int end_x, int end_y, int speed_sq, int *is_sand, int g_depth);

/**
 * @brief Explore l'arbre des actions possibles (A*) et décide du meilleur mouvement.
 * @param best_ax Pointeur vers le choix final d'accélération en X.
 * @param best_ay Pointeur vers le choix final d'accélération en Y.
 */
void decide_action_astar(int *best_ax, int *best_ay);

#endif /* ASTAR_H */