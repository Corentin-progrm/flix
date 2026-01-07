# 1. Variables
CC = gcc
# -Iinclude dit à GCC : "Cherche les fichiers .h dans le dossier include"
CFLAGS = -Wall -Wextra -g -Iinclude

# Dossiers
SRC_DIR = src
OBJ_DIR = build
# L'exécutable sera créé dans le dossier build
TARGET = $(OBJ_DIR)/netflix_perso.exe

# 2. Cible finale (L'exécutable)
# On a besoin des 3 fichiers objets correspondant à tes 3 fichiers .c
$(TARGET): $(OBJ_DIR)/main.o $(OBJ_DIR)/interface.o $(OBJ_DIR)/resultat.o
	$(CC) $(CFLAGS) -o $@ $^

# 3. Compilation de main.c
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c include/interface.h include/resultat.h
	$(CC) $(CFLAGS) -c $< -o $@

# 4. Compilation de interface.c
$(OBJ_DIR)/interface.o: $(SRC_DIR)/interface.c include/interface.h include/resultat.h
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Compilation de resultat.c (Je vois que tu as ajouté ce fichier)
$(OBJ_DIR)/resultat.o: $(SRC_DIR)/resultat.c include/resultat.h
	$(CC) $(CFLAGS) -c $< -o $@

# 6. Nettoyage (Commande Windows pour supprimer les fichiers générés)
clean:
	del $(OBJ_DIR)\*.o $(OBJ_DIR)\*.exe