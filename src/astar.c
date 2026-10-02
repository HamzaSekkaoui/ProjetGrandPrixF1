/**
 * @file astar.c
 * @brief Algorithme de recherche A* et gestion du Min-Heap.
 * @author Hamza SEKKAOUI
 * @author hamza hour
 * @author reda sarsri
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "astar.h"

Pos2Dint me, adv1, adv2;
int current_vx = 0, current_vy = 0;
int my_boosts = 5;
int my_gas = 0;

static Node *heap = NULL;
static int heap_size = 0;
static int *visited_gas = NULL;
static int *visited_tag = NULL;
static int current_tag = 0;

void init_astar(void) {
    heap = (Node *)malloc(MAX_NODES * sizeof(Node));
    visited_gas = (int *)malloc(map.width * map.height * 121 * sizeof(int));
    visited_tag = (int *)calloc(map.width * map.height * 121, sizeof(int));
}

void free_astar(void) {
    if (heap) free(heap);
    if (visited_gas) free(visited_gas);
    if (visited_tag) free(visited_tag);
}

/**
 * @brief Insère un nœud dans la file de priorité (Min-Heap).
 * @param n Le nœud à insérer.
 */
static void push_heap(Node n) {
    int i, p;
    if (heap_size >= MAX_NODES) return;
    i = heap_size++;
    while (i > 0) {
        p = (i - 1) / 2;
        if (heap[p].f <= n.f) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = n;
}

/**
 * @brief Extrait le nœud avec le plus petit coût du Min-Heap.
 * @return Le nœud ayant la valeur f minimale.
 */
static Node pop_heap(void) {
    Node ret = heap[0];
    Node temp = heap[--heap_size];
    int i = 0, child;
    while ((child = 2 * i + 1) < heap_size) {
        if (child + 1 < heap_size && heap[child + 1].f < heap[child].f) child++;
        if (temp.f <= heap[child].f) break;
        heap[i] = heap[child];
        i = child;
    }
    heap[i] = temp;
    return ret;
}

int is_valid_move(int start_x, int start_y, int end_x, int end_y, int speed_sq, int *is_sand, int g_depth) {
    InfoLine vline;
    Pos2Dint p;
    *is_sand = 0;
   
    if (end_x < 0 || end_x >= map.width || end_y < 0 || end_y >= map.height) return 0;
    if (map.grid[start_y][start_x] == '~' && speed_sq > 1) return 0;

    initLine(start_x, start_y, end_x, end_y, &vline);
    while (nextPoint(&vline, &p, +1) > 0) {
        if (p.x == start_x && p.y == start_y) continue;
        if (p.x < 0 || p.x >= map.width || p.y < 0 || p.y >= map.height) return 0;
        if (map.grid[p.y][p.x] == '.') return 0;
        if (map.grid[p.y][p.x] == '~') {
            *is_sand = 1;
            if (speed_sq > 1) return 0;
        }
       
        if (g_depth == 1) {
            if (p.x == adv1.x && p.y == adv1.y) return 0;
            if (p.x == adv2.x && p.y == adv2.y) return 0;
        }
    }
    return 1;
}

void decide_action_astar(int *best_ax, int *best_ay) {
    int ax, ay;
    int current_speed_sq, me_in_sand;
    clock_t start_time = clock();
    Node best_ever;
   
    best_ever.h = INF; best_ever.g = INF;
    *best_ax = 0; *best_ay = 0;
   
    current_tag++;
    if (current_tag > 2000000000) {
        memset(visited_tag, 0, map.width * map.height * 121 * sizeof(int));
        current_tag = 1;
    }
   
    heap_size = 0;
    current_speed_sq = current_vx * current_vx + current_vy * current_vy;
    me_in_sand = (map.grid[me.y][me.x] == '~');
   
    for (ax = -2; ax <= 2; ax++) {
        for (ay = -2; ay <= 2; ay++) {
            int is_boost, nvx, nvy, nx, ny, speed_sq, is_sand, gas_cost, new_gas, f, state_idx;
            double expected_gas;
           
            is_boost = (abs(ax) > 1 || abs(ay) > 1);
            if (is_boost && my_boosts <= 0) continue;
           
            nvx = current_vx + ax;
            nvy = current_vy + ay;
            nx = me.x + nvx;
            ny = me.y + nvy;
            speed_sq = nvx * nvx + nvy * nvy;
            is_sand = 0;
           
            if (speed_sq > 25) continue;
           
            if (is_valid_move(me.x, me.y, nx, ny, speed_sq, &is_sand, 1)) {
                gas_cost = (ax * ax + ay * ay) + (int)((3.0 * sqrt((double)current_speed_sq)) / 2.0) + me_in_sand;
                new_gas = my_gas - gas_cost;
               
                if (new_gas > 0) {
                    Node n;
                    n.x = nx; n.y = ny; n.vx = nvx; n.vy = nvy;
                    n.g = 1;
                    n.h = map.heuristic[ny][nx];
                    if (n.h == INF) n.h = 50000;
                    n.gas_left = new_gas;
                    n.gas_spent = gas_cost;
                   
                    f = n.g * 5000 + n.h * 1000;
                    expected_gas = n.h * 1.5;
                   
                    if ((double)new_gas < expected_gas) {
                        f += (int)((expected_gas - (double)new_gas) * 200.0);
                    }
                    f += gas_cost * 10;
                    if (is_boost) f += 500;
                   
                    n.f = f;
                    n.boosts_left = my_boosts - is_boost;
                    n.first_ax = ax; n.first_ay = ay;
                   
                    state_idx = (((ny * map.width) + nx) * 11 + (nvx + 5)) * 11 + (nvy + 5);
                    visited_tag[state_idx] = current_tag;
                    visited_gas[state_idx] = new_gas;
                    push_heap(n);
                }
            }
        }
    }
   
    while (heap_size > 0) {
        Node curr;
        int curr_speed_sq, curr_in_sand;
       
        curr = pop_heap();
       
        if (map.grid[curr.y][curr.x] == '=') {
            *best_ax = curr.first_ax;
            *best_ay = curr.first_ay;
            return;
        }
       
        if (curr.h < best_ever.h || (curr.h == best_ever.h && curr.g < best_ever.g)) {
            best_ever = curr;
        }
       
        if (((double)(clock() - start_time)) / CLOCKS_PER_SEC > 0.85) break;
        if (heap_size >= MAX_NODES - 50) break;
       
        curr_speed_sq = curr.vx * curr.vx + curr.vy * curr.vy;
        curr_in_sand = (map.grid[curr.y][curr.x] == '~');
       
        for (ax = -2; ax <= 2; ax++) {
            for (ay = -2; ay <= 2; ay++) {
                int is_boost, nvx, nvy, nx, ny, speed_sq, is_sand, gas_cost, new_gas;
               
                is_boost = (abs(ax) > 1 || abs(ay) > 1);
                if (is_boost && curr.boosts_left <= 0) continue;
               
                nvx = curr.vx + ax; nvy = curr.vy + ay;
                nx = curr.x + nvx; ny = curr.y + nvy;
                speed_sq = nvx * nvx + nvy * nvy;
                is_sand = 0;
               
                if (speed_sq > 25) continue;
               
                if (is_valid_move(curr.x, curr.y, nx, ny, speed_sq, &is_sand, curr.g + 1)) {
                    gas_cost = (ax * ax + ay * ay) + (int)((3.0 * sqrt((double)curr_speed_sq)) / 2.0) + curr_in_sand;
                    new_gas = curr.gas_left - gas_cost;
                   
                    if (new_gas > 0) {
                        int state_idx = (((ny * map.width) + nx) * 11 + (nvx + 5)) * 11 + (nvy + 5);
                       
                        if (visited_tag[state_idx] != current_tag || new_gas > visited_gas[state_idx]) {
                            int h, g, f;
                            double expected_gas;
                            Node n;
                           
                            visited_tag[state_idx] = current_tag;
                            visited_gas[state_idx] = new_gas;
                           
                            h = map.heuristic[ny][nx];
                            if (h == INF) h = 50000;
                            g = curr.g + 1;
                           
                            f = g * 5000 + h * 1000;
                            expected_gas = h * 1.5;
                           
                            if ((double)new_gas < expected_gas) {
                                f += (int)((expected_gas - (double)new_gas) * 200.0);
                            }
                            f += (curr.gas_spent + gas_cost) * 10;
                            if (is_boost) f += 500;
                           
                            if (nx == adv1.x && ny == adv1.y) f += 20000;
                            if (nx == adv2.x && ny == adv2.y) f += 20000;
                           
                            n.x = nx; n.y = ny; n.vx = nvx; n.vy = nvy;
                            n.g = g; n.h = h; n.gas_left = new_gas; n.gas_spent = curr.gas_spent + gas_cost; n.f = f;
                            n.boosts_left = curr.boosts_left - is_boost;
                            n.first_ax = curr.first_ax; n.first_ay = curr.first_ay;
                            push_heap(n);
                        }
                    }
                }
            }
        }
    }
   
    if (best_ever.h != INF) {
        *best_ax = best_ever.first_ax;
        *best_ay = best_ever.first_ay;
    } else {
        *best_ax = (current_vx > 0) ? -1 : ((current_vx < 0) ? 1 : 0);
        *best_ay = (current_vy > 0) ? -1 : ((current_vy < 0) ? 1 : 0);
    }
}