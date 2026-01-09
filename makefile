# 1. Variables
CC = gcc

# --- CORRECTION 1 : LES HEADERS (.h) ---
# -Iinclude      : Cherche tes .h perso (modele.h...)
# -Ilib/include  : Cherche raylib.h dans ton dossier lib/include
CFLAGS = -Wall -Wextra -g -Iinclude -Ilib/include

# --- CORRECTION 2 : LES LIBRAIRIES (.a) ---
# -Llib/lib      : Cherche le fichier libraylib.a dans ton dossier lib/lib
LDFLAGS = -Llib/lib

# Bibliothèques Windows obligatoires pour Raylib
# -lraylib : Le fichier principal
# -lgdi32 -lwinmm : Des outils graphiques de Windows
LIBS = -lraylib -lgdi32 -lwinmm

# Dossiers
SRC_DIR = src
OBJ_DIR = build
TARGET = $(OBJ_DIR)/netflix_perso.exe

# 2. Cible finale (L'exécutable)
# On ajoute $(LDFLAGS) et $(LIBS) à la fin pour lier Raylib
$(TARGET): $(OBJ_DIR)/main.o $(OBJ_DIR)/affichage.o $(OBJ_DIR)/modele.o $(OBJ_DIR)/storage.o $(OBJ_DIR)/utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

# 3. Compilation de main.c
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c include/affichage.h include/storage.h include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

# 4. Compilation de affichage.c
$(OBJ_DIR)/affichage.o: $(SRC_DIR)/affichage.c include/affichage.h include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Compilation des autres modules
$(OBJ_DIR)/modele.o: $(SRC_DIR)/modele.c include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/storage.o: $(SRC_DIR)/storage.c include/storage.h include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c include/utils.h
	$(CC) $(CFLAGS) -c $< -o $@

# 6. Nettoyage
clean:
	del $(OBJ_DIR)\*.o $(OBJ_DIR)\*.exe