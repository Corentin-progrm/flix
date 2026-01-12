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

    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue);

    // ETAT : Quel bouton est actif ? (-1 = Aucun/Tout)
    int filtreSelectionne = -1;

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
            
            ClearBackground(GetColor(0x141414FF)); 
            dessinerEnTete();

            // 1. MENU : On récupère le clic
            int clicMenu = dessinerBarreCategories();
            
            // Si on clique sur un bouton, on met à jour le filtre
            if (clicMenu != -1) {
                // Petit hack : Si on clique sur le même bouton, on désactive le filtre (toggle)
                if (filtreSelectionne == clicMenu) filtreSelectionne = -1;
                else filtreSelectionne = clicMenu;
                
                printf("Filtre change : %d\n", filtreSelectionne);
            }

            // 2. GRILLE : On appelle notre nouvelle fonction magique
            dessinerGrilleFiltree(catalogue, filtreSelectionne);

        EndDrawing();
    }

    libererTexturesCatalogue();
    freeCatalogue(catalogue);
    fermerInterface();

    return 0;
}