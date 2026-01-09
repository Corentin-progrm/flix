/**
 * Module main - main.c
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module main est le point d'entrée du programme.
**/

/* LIBRARY ================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include "modele.h"
#include "affichage.h"
#include "storage.h"
#include "utils.h"

/* DEFINITION DES FONCTIONS =============================================== */

/**
 * Module main - main.c
 * Par : Corentin Couëron
 */


int main(void)
{
    // 1. Initialisation de la fenêtre (Taille plus grande pour la grille)
    initInterface(1024, 768, "NounaFlix");

    // 2. Animation de démarrage (Bloque le programme tant que pas fini)
    animLogoStart();

    // 3. Chargement des données APRES l'animation
    // Cela permet une transition fluide vers le menu
    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue); // Charge les images

    // 4. Boucle Principale (Grille des films)
    while (!WindowShouldClose()) 
    {
        BeginDrawing();
            
            ClearBackground(RAYWHITE); 
            dessinerFondMenu();

            // --- Logique de la Grille ---
            int nbFilms = getNbMedia(catalogue);
            int startX = 40;
            int startY = 100;
            
            // Calcul du nombre de colonnes dynamique
            int largeurFenetre = GetScreenWidth();
            int espaceTotalCarte = CARTE_LARGEUR + CARTE_PADDING;
            int colonnesMax = (largeurFenetre - (startX * 2)) / espaceTotalCarte;
            if (colonnesMax < 1) colonnesMax = 1;

            // Affichage de chaque film
            for (int i = 0; i < nbFilms; i++) {
                t_media m = getMediaCatalogue(catalogue, i);
                
                int colonne = i % colonnesMax;
                int ligne = i / colonnesMax;

                Rectangle rectCarte;
                rectCarte.x = (float)(startX + colonne * espaceTotalCarte);
                rectCarte.y = (float)(startY + ligne * (CARTE_HAUTEUR + CARTE_PADDING));
                rectCarte.width = (float)CARTE_LARGEUR;
                rectCarte.height = (float)CARTE_HAUTEUR;

                // On dessine la carte et on vérifie le clic en même temps
                // (On verifie que mesTextures n'est pas NULL par sécurité)
                if (mesTextures != NULL) {
                    if (dessinerCarteMedia(rectCarte, m, mesTextures[i])) {
                        printf("Lancement du film : %s\n", getTitre(m));
                        lancerVideo(m);
                    }
                }
            }

        EndDrawing();
    }

    // 5. Nettoyage
    libererTexturesCatalogue();
    freeCatalogue(catalogue);
    fermerInterface();

    return 0;
}





/*

int main(void) {
    // 1. Chargement des données
    t_catalogue catalogue = chargerBaseDeDonnees();
    
    // 2. Initialisation de la fenêtre (Largeur, Hauteur, Titre)
    initInterface(1024, 768, "Netflix Perso - Grid Edition");

    // 3. Chargement des images (Textures)
    // IMPORTANT : Doit être fait APRES initInterface() car OpenGL doit être prêt
    chargerTexturesCatalogue(catalogue);

    // 4. Boucle principale
    while (!WindowShouldClose()) {

        // --- DESSIN ---
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            
            // Affiche le titre "NETFLIX PERSO"
            dessinerFondMenu();

            // --- LOGIQUE DE LA GRILLE ---
            int nbFilms = getNbMedia(catalogue);
            
            // Paramètres de positionnement
            int startX = 40;
            int startY = 100;
            
            // Calcul dynamique des colonnes selon la largeur de la fenêtre
            // (Evite que ça déborde si tu changes la taille de fenêtre)
            int largeurFenetre = GetScreenWidth();
            int espaceTotalCarte = CARTE_LARGEUR + CARTE_PADDING;
            
            // Combien de cartes tiennent en largeur ?
            int colonnesMax = (largeurFenetre - (startX * 2)) / espaceTotalCarte;
            
            // Sécurité : au moins 1 colonne pour éviter la division par zéro
            if (colonnesMax < 1) colonnesMax = 1;

            // Boucle d'affichage des films
            for (int i = 0; i < nbFilms; i++) {
                t_media m = getMediaCatalogue(catalogue, i);
                
                // Calcul mathématique de la position (Ligne / Colonne)
                int colonne = i % colonnesMax;
                int ligne = i / colonnesMax;

                // Création du rectangle de la carte
                Rectangle rectCarte;
                rectCarte.x = (float)(startX + colonne * espaceTotalCarte);
                rectCarte.y = (float)(startY + ligne * (CARTE_HAUTEUR + CARTE_PADDING));
                rectCarte.width = (float)CARTE_LARGEUR;
                rectCarte.height = (float)CARTE_HAUTEUR;

                // --- AFFICHAGE DU WIDGET ---
                // On vérifie que les textures existent bien avant de les utiliser
                if (mesTextures != NULL) {
                    if (dessinerCarteMedia(rectCarte, m, mesTextures[i])) {
                        printf("Clic sur le film : %s\n", getTitre(m));
                        lancerVideo(m);
                    }
                }
            }

        EndDrawing();
    }

    // 5. Nettoyage final
    libererTexturesCatalogue(); // Décharge les images de la carte graphique
    fermerInterface();          // Ferme la fenêtre Raylib
    freeCatalogue(catalogue);   // Vide la mémoire du catalogue
    
    return 0;
}



*/