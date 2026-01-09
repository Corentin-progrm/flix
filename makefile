# 1. Variables
CC = gcc
# -Iinclude dit à GCC : "Cherche les fichiers .h dans le dossier include"
CFLAGS = -Wall -Wextra -g -Iinclude

# Dossiers
SRC_DIR = src
OBJ_DIR = build
LIBS = -lraylib -lgdi32 -lwinmm
# L'exécutable sera créé dans le dossier build
TARGET = $(OBJ_DIR)/netflix_perso.exe

# 2. Cible finale (L'exécutable)
# On a besoin des 3 fichiers objets correspondant à tes 3 fichiers .c
$(TARGET): $(OBJ_DIR)/main.o $(OBJ_DIR)/affichage.o $(OBJ_DIR)/modele.o $(OBJ_DIR)/storage.o $(OBJ_DIR)/utils.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# 3. Compilation de main.c
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c include/affichage.h include/storage.h include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

# 4. Compilation de affichage.c
$(OBJ_DIR)/affichage.o: $(SRC_DIR)/affichage.c include/affichage.h include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Compilation de modele.c 
$(OBJ_DIR)/modele.o: $(SRC_DIR)/modele.c include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Compilation de storage.c
$(OBJ_DIR)/storage.o: $(SRC_DIR)/storage.c include/storage.h include/modele.h
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Compilation de utils.c
$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c include/utils.h
	$(CC) $(CFLAGS) -c $< -o $@

# 6. Nettoyage (Commande Windows pour supprimer les fichiers générés)
clean:
	del $(OBJ_DIR)\*.o $(OBJ_DIR)\*.exe