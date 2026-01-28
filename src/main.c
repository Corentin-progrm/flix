/**
 * Module main - main.c
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module main est le point d'entrée du programme.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modele.h"
#include "affichage.h"
#include "storage.h"

// États de l'application
#define ETAT_ACCUEIL 0
#define ETAT_DETAILS 1

// Configuration
#define LARGEUR_FENETRE 1024
#define HAUTEUR_FENETRE 768
#define HAUTEUR_HEADER 180
#define TAILLE_MAX_RECHERCHE 64

// Utilisation de la couleur de fond centralisée
const Color COLOR_BG_APP = { 25, 46, 3, 255 }; // Background application

int main(void)
{
    // Initialisation
    initInterface(LARGEUR_FENETRE, HAUTEUR_FENETRE, "NounaFlix");
    //animLogoStart();

    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue);

    // États de l'application
    int etatApp = ETAT_ACCUEIL;
    int indexFilmChoisi = -1;
    
    int filtreSelectionne = -1;
    char bufferRecherche[TAILLE_MAX_RECHERCHE] = {0};
    int modeRecherche = 1;

    // Boucle principale
    while (!WindowShouldClose()) 
    {
        // Gestion de la saisie de recherche
        if (modeRecherche && etatApp == ETAT_ACCUEIL) {
            int key = GetCharPressed();
            
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (strlen(bufferRecherche) < TAILLE_MAX_RECHERCHE - 1)) {
                    int len = strlen(bufferRecherche);
                    bufferRecherche[len] = (char)key;
                    bufferRecherche[len + 1] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = strlen(bufferRecherche);
                if (len > 0) bufferRecherche[len - 1] = '\0';
            }
        }

        // Rendu
        BeginDrawing();
        ClearBackground(COLOR_BG_APP); 

        if (etatApp == ETAT_ACCUEIL) {
            
            // Grille de films (arrière-plan)
            int idClique = dessinerGrilleFiltree(catalogue, filtreSelectionne, bufferRecherche);
            
            if (idClique != -1) {
                indexFilmChoisi = idClique;
                etatApp = ETAT_DETAILS;
            }

            // Cache pour masquer le scroll
            DrawRectangle(0, 0, GetScreenWidth(), HAUTEUR_HEADER, COLOR_BG_APP);

            // Interface fixe (header + menu + recherche)
            dessinerEnTete();

            int clicMenu = dessinerBarreCategories();
            if (clicMenu != -1 && clicMenu != 1) {
                filtreSelectionne = (filtreSelectionne == clicMenu) ? -1 : clicMenu;
            }

            if (modeRecherche) {
                dessinerBarreRecherche(bufferRecherche);
            }
        }
        else if (etatApp == ETAT_DETAILS) {
            
            if (indexFilmChoisi >= 0) {
                t_media m = getMediaCatalogue(catalogue, indexFilmChoisi);
                int action = dessinerPageDetails(m, mesTextures[indexFilmChoisi]);

                if (action == 1) etatApp = ETAT_ACCUEIL;
                if (action == 2) lancerVideo(m);
            }
        }

        EndDrawing();
    }

    // Nettoyage
    libererTexturesCatalogue();
    freeCatalogue(catalogue);
    fermerInterface();

    return 0;
}