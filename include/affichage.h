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
#include "lib/include/raylib.h"

/* DECLARATION DES FONCTIONS ============================================== */

// Affiche le titre du programme
int afficherBaniere(void);

// Affiche l'acceuil du programme
int afficherCatalogue(t_catalogue catalogue);

// Affichage detaille d'un media
void afficherMedia(t_media media);

// Affiche le menu et retourne le choix de l'utilisateur
int afficherMenuAcceuil(void);
int afficherMenuMedia(void);
int afficherMenuRecherche(void);

// Affiche le menu de recherche des films
void rechercherFilmParTitre(t_catalogue catalogue);


/* INTERFACE GRAPHIQUE ==================================================== */

// Ouvre la fenêtre et règle les FPS
void initInterface(int largeur, int hauteur, char* titre);

// Ferme la fenêtre proprement
void fermerInterface(void);

// Dessine tout le contenu de l'accueil (Textes, Liste...)
void dessinerMenuPrincipal(t_catalogue catalogue);




#endif