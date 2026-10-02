/**
 * @file debug.h
 * @brief Fonctions de journalisation et de débogage.
 * @author Hamza SEKKAOUI
 * @author hamza hour
 * @author reda sarsri
 */

#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_MODE 1

/**
 * @brief Fonction de débogage (compatible C89).
 * N'affiche les messages sur stderr que si DEBUG_MODE vaut 1.
 * @param format Chaîne de formatage (style printf).
 * @param ... Arguments variables.
 */
void debug_log(const char *format, ...);

#endif /* DEBUG_H */