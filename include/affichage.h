/**
 * Module affichage - affichage.h
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module affichage gère toutes les fonctions
 *               d'affichage du programme, y compris les menus
 *              et les détails des medias.
**/

#ifndef AFFICHAGE_H
#define AFFICHAGE_H

/* LIBRARY ================================================================ */
#include "modele.h"

/* DECLARATION DES FONCTIONS ============================================== */

// Affiche l'acceuil du programme
void afficherAccueil(t_catalogue catalogue);

// Affichage detaille d'un media
void afficherMedia(t_media media);

// Affiche le menu et retourne le choix de l'utilisateur
int afficherMenuAcceuil(void);
int afficherMenuMedia(void);

#endif