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
#include "storage.h"
#include "raylib.h"

/* CONSTANTES ============================================================= */
#define CARTE_LARGEUR 160
#define CARTE_HAUTEUR 220
#define CARTE_PADDING 20

/* DECLARATION DES FONCTIONS ============================================== */

// Variable globale pour l'accès aux textures (externe)
extern Texture2D* mesTextures;

// --- GESTION DE L'INTERFACE (Cycle de vie) ---
void initInterface(int largeur, int hauteur, char* titre);
void fermerInterface(void);

// --- GESTION DES RESSOURCES ---
void chargerTexturesCatalogue(t_catalogue catalogue);
void libererTexturesCatalogue(void);

// --- ÉLÉMENTS GRAPHIQUES (Widgets) ---

// Affiche le fond, le logo et le titre
void dessinerEnTete(void);

// Affiche la barre de menus (catégories) et retourne l'index cliqué (-1 si rien)
int dessinerBarreCategories(void);

// Affiche une carte de film et retourne 1 si cliquée
int dessinerCarteMedia(Rectangle rect, t_media m, Texture2D miniature);

// Affiche la grille de médias filtrée selon le filtre actif
int dessinerGrilleFiltree(t_catalogue catalogue, int filtreActif, char* recherche);

// Affiche la barre de recherche avec le texte courant
void dessinerBarreRecherche(char* bufferTexte, int estActif);

// Affiche la page de détails d'un média.
int dessinerPageDetails(t_media m, Texture2D affiche);

// --- ANIMATION ---
void animLogoStart(void);

#endif