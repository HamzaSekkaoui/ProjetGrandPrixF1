/**
 * @file main.c
 * @brief Pilote autonome pour le Challenge Grand Prix F1 (ENSICAEN).
 * @author Hamza SEKKAOUI
 * @author hamza hour
 * @author reda sarsri
 * 
 * Implémentation basée sur l'algorithme A* (A-Star) pour déterminer
 * le meilleur vecteur d'accélération en tenant compte de l'essence, 
 * du sable, de l'inertie et des adversaires.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "follow_line.h"
#include "map.h"
#include "astar.h"
#include "debug.h"

/**
 * @brief Point d'entrée principal. Gère les I/O avec le gestionnaire de course (GDC).
 * @return EXIT_SUCCESS à la fin de la course.
 */
int main(void) {
    char line_buffer[MAX_LINE_LENGTH];
    int ax, ay;
    int last_x = -1, last_y = -1;
    int round_num = 0;

    read_map();
    my_gas = map.initialGas;
    compute_heuristic();
    init_astar();

    debug_log("=== INITIALISATION TERMINEE ===\n");
    debug_log("MAP: %dx%d | ESSENCE INITIALE: %d\n", map.width, map.height, my_gas);

    while (!feof(stdin)) {
        int speed_sq_before, sand_penalty;
       
        if (fgets(line_buffer, MAX_LINE_LENGTH, stdin) == NULL) break;
        sscanf(line_buffer, "%d %d %d %d %d %d",
               &me.x, &me.y, &adv1.x, &adv1.y, &adv2.x, &adv2.y);
       
        round_num++;
        debug_log("\n--- ROUND %d ---\n", round_num);
        debug_log("Ma Position: (%d, %d) | Gas: %d | Boosts: %d\n", me.x, me.y, my_gas, my_boosts);
       
        if (last_x != -1) {
            if (me.x != last_x + current_vx || me.y != last_y + current_vy) {
                debug_log("[ATTENTION] Crash ou Reset GDC detecte! Reset de la vitesse.\n");
                current_vx = 0; current_vy = 0;
            }
        }
       
        decide_action_astar(&ax, &ay);
       
        speed_sq_before = current_vx * current_vx + current_vy * current_vy;
        sand_penalty = (map.grid[me.y][me.x] == '~') ? 1 : 0;
        my_gas -= (ax * ax + ay * ay) + (int)((3.0 * sqrt((double)speed_sq_before)) / 2.0) + sand_penalty;
       
        current_vx += ax;
        current_vy += ay;
       
        if (abs(ax) > 1 || abs(ay) > 1) my_boosts--;
        last_x = me.x; last_y = me.y;
       
        debug_log("Action Choisie: Acc=(%d, %d) | Nouvelle Vitesse=(%d, %d)\n", ax, ay, current_vx, current_vy);
       
        fprintf(stdout, "%d %d\n", ax, ay);
        fflush(stdout);
    }
   
    free_astar();
    free_map();
    return EXIT_SUCCESS;
}