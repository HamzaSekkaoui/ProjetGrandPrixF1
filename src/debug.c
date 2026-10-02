/**
 * @file debug.c
 * @brief Fonctions d'affichage des logs.
 * @author Hamza SEKKAOUI
 * @author hamza hour
 * @author reda sarsri
 */

#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

void debug_log(const char *format, ...) {
#if DEBUG_MODE
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fflush(stderr);
#else
    (void)format;
#endif
}