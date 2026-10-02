# ====================================================================
# Makefile - Challenge Grand Prix F1 (ENSICAEN)
# Auteurs: Hamza SEKKAOUI, hamza hour, reda sarsri
# ====================================================================

EXE = yellowFlash

# Répertoires
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
DRIVERS_DIR = ../drivers

# Compilateur et options ANSI C (C89)
CC = gcc
CFLAGS = -ansi -pedantic -Wall -Wextra -O3 -I$(INC_DIR)
DEBUG_FLAGS = -ansi -pedantic -Wall -Wextra -g -O0 -fsanitize=address -I$(INC_DIR)
LDFLAGS = -lm

TARGET = $(BIN_DIR)/$(EXE)

# Liste des fichiers sources et objets
SRCS = $(SRC_DIR)/follow_line.c \
       $(SRC_DIR)/debug.c \
       $(SRC_DIR)/map.c \
       $(SRC_DIR)/astar.c \
       $(SRC_DIR)/main.c

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all clean distclean debug doc install

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Copie vers le répertoire des pilotes (GDC)
install: all
	@mkdir -p $(DRIVERS_DIR)
	cp $(TARGET) $(DRIVERS_DIR)/$(EXE)

# Documentation Doxygen
doc:
	doxygen doxyfile

# Cible spéciale pour le Debug avec AddressSanitizer
debug: clean
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)
	$(CC) $(DEBUG_FLAGS) -c $(SRC_DIR)/follow_line.c -o $(OBJ_DIR)/follow_line.o
	$(CC) $(DEBUG_FLAGS) -c $(SRC_DIR)/debug.c -o $(OBJ_DIR)/debug.o
	$(CC) $(DEBUG_FLAGS) -c $(SRC_DIR)/map.c -o $(OBJ_DIR)/map.o
	$(CC) $(DEBUG_FLAGS) -c $(SRC_DIR)/astar.c -o $(OBJ_DIR)/astar.o
	$(CC) $(DEBUG_FLAGS) -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o
	$(CC) $(DEBUG_FLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR)

distclean: clean
	rm -rf $(BIN_DIR)
	rm -f $(DRIVERS_DIR)/$(EXE)
	rm -rf html/ latex/ doc/