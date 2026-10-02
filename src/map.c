/**
 * @file map.c
 * @brief Implémentation du chargement de circuit et du calcul BFS.
 * @author Hamza SEKKAOUI
 * @author hamza hour
 * @author reda sarsri
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"

Map map;

void read_map(void) {
    char line_buffer[MAX_LINE_LENGTH];
    int i;
   
    if (fgets(line_buffer, MAX_LINE_LENGTH, stdin) == NULL) return;
    sscanf(line_buffer, "%d %d %d", &map.width, &map.height, &map.initialGas);
   
    map.grid = (char **)malloc(map.height * sizeof(char *));
    map.heuristic = (int **)malloc(map.height * sizeof(int *));
   
    for (i = 0; i < map.height; i++) {
        map.grid[i] = (char *)malloc(map.width + 2);
        map.heuristic[i] = (int *)malloc(map.width * sizeof(int));
       
        if (fgets(line_buffer, MAX_LINE_LENGTH, stdin) == NULL) break;
        strncpy(map.grid[i], line_buffer, map.width);
        map.grid[i][map.width] = '\0';
    }
}

void compute_heuristic(void) {
    int head = 0, tail = 0;
    int i, j, dx, dy, nx, ny, current, is_diag, weight;
    Pos2Dint *queue = (Pos2Dint *)malloc(map.width * map.height * sizeof(Pos2Dint));
   
    for (i = 0; i < map.height; i++) {
        for (j = 0; j < map.width; j++) {
            if (map.grid[i][j] == '=') {
                queue[tail].x = j;
                queue[tail].y = i;
                tail++;
                map.heuristic[i][j] = 0;
            } else {
                map.heuristic[i][j] = INF;
            }
        }
    }
   
    while (head < tail) {
        Pos2Dint p = queue[head++];
        current = map.heuristic[p.y][p.x];
       
        for (dx = -1; dx <= 1; dx++) {
            for (dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                nx = p.x + dx;
                ny = p.y + dy;
               
                if (nx >= 0 && nx < map.width && ny >= 0 && ny < map.height) {
                    if (map.grid[ny][nx] == '.') continue;
                   
                    is_diag = (dx != 0 && dy != 0);
                    if (is_diag && (map.grid[p.y][p.x] == '~' || map.grid[ny][nx] == '~')) continue;
                   
                    weight = (map.grid[p.y][p.x] == '~') ? 5 : 1;
                   
                    if (current + weight < map.heuristic[ny][nx]) {
                        map.heuristic[ny][nx] = current + weight;
                        queue[tail].x = nx;
                        queue[tail].y = ny;
                        tail++;
                    }
                }
            }
        }
    }
    free(queue);
}

void free_map(void) {
    int i;
    for (i = 0; i < map.height; i++) {
        free(map.grid[i]);
        free(map.heuristic[i]);
    }
    free(map.grid);
    free(map.heuristic);
}