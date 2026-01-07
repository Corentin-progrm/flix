#ifndef INTERFACE_H
#define INTERFACE_H

#include "resultat.h"

// Affiche le menu et retourne le choix de l'utilisateur
int afficherMenu(void);

// Charge le fichier texte, vérifie les .mp4, et retourne TOUTE la structure remplie
t_resultat chargerBaseDeDonnees(void);

// Reçoit la structure remplie et l'affiche proprement à l'écran
void afficherCatalogue(t_resultat resultat);

// Utilitaire
int fichierExiste(char *chemin);

#endif