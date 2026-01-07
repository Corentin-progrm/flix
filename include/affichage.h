#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "modele.h"

// Affiche l'acceuil du programme
void afficherAccueil(t_catalogue catalogue);

// Affichage detaille d'un media
void afficherMedia(t_media media);

// Affiche le menu et retourne le choix de l'utilisateur
int afficherMenuAcceuil(void);
int afficherMenuMedia(void);


#endif