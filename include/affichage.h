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
#include "raylib.h"

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

void initInterface(int largeur, int hauteur, char* titre);
void fermerInterface(void);

// Affiche un bouton et renvoie 1 (true) s'il est cliqué
int dessinerBouton(Rectangle rect, char* texte, Color couleurBase);

// On garde ta fonction principale, mais elle ne prendra plus rien en paramètre pour l'instant
// car c'est le main qui va orchestrer les boutons.
void dessinerFondMenu(void);




#endif