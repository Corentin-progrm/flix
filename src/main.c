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

int main(void)
{
    // 1. Initialisation
    initInterface(1024, 768, "NounaFlix");
    
    // Animation d'intro
    animLogoStart();

    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue);

    // 2. Gestion des États
    int etatApp = 0;                // 0 = ACCUEIL, 1 = DETAILS
    int indexFilmChoisi = -1;       // Quel film afficher en détail
    
    int filtreSelectionne = -1;     // Filtre catégorie
    char bufferRecherche[64] = {0}; // Texte recherche
    int modeRecherche = 0;          // Barre de recherche visible ou non

    while (!WindowShouldClose()) 
    {

        if (modeRecherche && etatApp == 0) {
            // 1. Récupérer les caractères saisis
            int key = GetCharPressed();
            
            while (key > 0) {
                // On accepte uniquement les caractères imprimables (ASCII 32 à 125)
                // et on vérifie qu'il reste de la place dans le buffer (max 63 chars)
                if ((key >= 32) && (key <= 125) && (strlen(bufferRecherche) < 63)) {
                    int len = strlen(bufferRecherche);
                    bufferRecherche[len] = (char)key;
                    bufferRecherche[len + 1] = '\0'; // Toujours terminer par \0
                }
                key = GetCharPressed(); // On vérifie s'il y a d'autres touches en attente
            }

            // 2. Gestion de la touche Effacer (Backspace)
            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = strlen(bufferRecherche);
                if (len > 0) bufferRecherche[len - 1] = '\0'; // On raccourcit la chaîne
            }
        }

        // --- AFFICHAGE ---
        BeginDrawing();
            
            ClearBackground(GetColor(0x141414FF)); 

            // === PAGE D'ACCUEIL ===
            if (etatApp == 0) {
                
                // ---------------------------------------------------------
                // ÉTAPE 1 : LA COUCHE DU FOND (Les Films)
                // ---------------------------------------------------------
                // On dessine la grille en PREMIER.
                // Si le ScissorMode rate ou si ça dépasse, ce n'est pas grave,
                // car on va redessiner par-dessus juste après.
                
                int idClique = dessinerGrilleFiltree(catalogue, filtreSelectionne, bufferRecherche);
                
                if (idClique != -1) {
                    indexFilmChoisi = idClique;
                    etatApp = 1;
                }

                // ---------------------------------------------------------
                // ÉTAPE 2 : LE "CACHE" OPAQUE
                // ---------------------------------------------------------
                // On dessine un rectangle de la couleur du fond qui couvre TOUTE
                // la zone du haut (Header + Menu + Recherche).
                // Hauteur 180 = C'est ton startY défini dans affichage.c
                DrawRectangle(0, 0, GetScreenWidth(), 180, GetColor(0x141414FF));

                // ---------------------------------------------------------
                // ÉTAPE 3 : L'INTERFACE FIXE (Header + Menu + Recherche)
                // ---------------------------------------------------------
                
                // A. L'En-tête (Logo + Titre)
                dessinerEnTete();

                // B. Le Menu Catégories
                int clicMenu = dessinerBarreCategories();
                if (clicMenu != -1) {
                    if (clicMenu == 1) { // Bouton Search
                        modeRecherche = !modeRecherche;
                        if (modeRecherche) filtreSelectionne = -1;
                    } else { // Bouton Catégorie
                        filtreSelectionne = (filtreSelectionne == clicMenu) ? -1 : clicMenu;
                    }
                }

                // C. La Barre de Recherche (si active)
                if (modeRecherche) {
                    dessinerBarreRecherche(bufferRecherche, 1);
                }
            }

            // === PAGE DETAILS (Reste inchangé car il couvre tout l'écran) ===
            else if (etatApp == 1) {
                if (indexFilmChoisi >= 0) {
                    t_media m = getMediaCatalogue(catalogue, indexFilmChoisi);
                    int action = dessinerPageDetails(m, mesTextures[indexFilmChoisi]);

                    if (action == 1) etatApp = 0;
                    if (action == 2) lancerVideo(m);
                }
            }

        EndDrawing();
    }
// ... fin du main identique ...

    // 4. Nettoyage
    libererTexturesCatalogue();
    freeCatalogue(catalogue);
    fermerInterface();

    return 0;
}