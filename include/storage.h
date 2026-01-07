#ifndef STORAGE_H
#define STORAGE_H

#include "modele.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>



// Charge le fichier texte, vérifie les .mp4, et retourne TOUTE la structure remplie
t_catalogue chargerBaseDeDonnees(void);

// Utilitaire
int fichierExiste(char *chemin);

void lancerVideo(t_media media);

#endif