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

int main(void)
{

    initInterface(1024, 768, "NounaFlix");

    animLogoStart();

    // 3. Chargement des données APRES l'animation
    // Cela permet une transition fluide vers le menu
    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue); // Charge les images

    // 4. Boucle Principale (Grille des films)
    while (!WindowShouldClose()) 
    {
        BeginDrawing();
            
            ClearBackground(BLACK); 
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