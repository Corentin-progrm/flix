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
    // animLogoStart(); // (Je commente pour tester plus vite, décommente-le)

    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue);

    // ETAT DU PROGRAMME
    int filtreSelectionne = -1;
    
    // ETAT DE LA RECHERCHE
    char bufferRecherche[64] = { 0 }; // Texte vide au début (\0 partout)
    int modeRecherche = 0;            // 0 = Caché, 1 = Actif

    while (!WindowShouldClose()) 
    {
        // --- GESTION INPUT CLAVIER (Si la recherche est active) ---
        if (modeRecherche) {
            // 1. Récupérer la touche appuyée
            int key = GetCharPressed();

            // Tant qu'il y a des touches dans la file d'attente
            while (key > 0) {
                // On accepte seulement les lettres/chiffres et si on a de la place
                if ((key >= 32) && (key <= 125) && (strlen(bufferRecherche) < 63)) {
                    int len = strlen(bufferRecherche);
                    bufferRecherche[len] = (char)key;
                    bufferRecherche[len + 1] = '\0'; // Toujours fermer la chaine
                }
                key = GetCharPressed(); // Touche suivante
            }

            // 2. Gestion de l'effacement (Backspace)
            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = strlen(bufferRecherche);
                if (len > 0) {
                    bufferRecherche[len - 1] = '\0';
                }
            }
        }

        // --- DESSIN ---
        BeginDrawing();
            
            ClearBackground(GetColor(0x141414FF)); 
            dessinerEnTete();

            // 1. MENU
            int clicMenu = dessinerBarreCategories();
            
            if (clicMenu != -1) {
                // Si on clique sur "Search" (Bouton 1)
                if (clicMenu == 1) {
                    // On bascule le mode recherche (On/Off)
                    modeRecherche = !modeRecherche;
                    
                    // Optionnel : On vide le filtre catégorie pour chercher dans TOUT
                    if (modeRecherche) filtreSelectionne = -1; 
                } 
                else {
                    // Si on clique sur un autre bouton (Film/Serie...), on applique le filtre
                    if (filtreSelectionne == clicMenu) filtreSelectionne = -1;
                    else filtreSelectionne = clicMenu;
                    
                    // On désactive la recherche textuelle si on change de catégorie (choix design)
                    // modeRecherche = 0; 
                }
            }

            // 2. BARRE DE RECHERCHE (Seulement si active)
            if (modeRecherche) {
                dessinerBarreRecherche(bufferRecherche, 1);
            }

            // 3. GRILLE
            // On passe maintenant DEUX filtres : la catégorie ET le texte
            dessinerGrilleFiltree(catalogue, filtreSelectionne, bufferRecherche);

        EndDrawing();
    }

    libererTexturesCatalogue();
    freeCatalogue(catalogue);
    fermerInterface();

    return 0;
}