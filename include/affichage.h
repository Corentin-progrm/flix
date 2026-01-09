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

// Dimensions des cartes (Tu pourras changer ça plus tard)
#define CARTE_LARGEUR 160
#define CARTE_HAUTEUR 220
#define CARTE_PADDING 20 // Espace entre les cartes

extern Texture2D* mesTextures;

void initInterface(int largeur, int hauteur, char* titre);
void fermerInterface(void);

// NOUVEAU : Charge toutes les images correspondant au catalogue en mémoire graphique
void chargerTexturesCatalogue(t_catalogue catalogue);

// NOUVEAU : Libère la mémoire graphique à la fin
void libererTexturesCatalogue(void);

void dessinerFondMenu(void);

// NOUVEAU : Dessine UNE carte cliquable (Image + Titre)
// Retourne 1 si cliquée
int dessinerCarteMedia(Rectangle rect, t_media m, Texture2D miniature);

// NOUVEAU : Fonction utilitaire pour dessiner l'image proportionnellement dans la carte
void DrawTextureScaled(Texture2D texture, Rectangle destRect);


void animLogoStart(void);



#endif