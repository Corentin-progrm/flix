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

int main() {
    // 1. Chargement des données (Comme avant)
    t_catalogue catalogue = chargerBaseDeDonnees();

    // 2. Initialisation Graphique (Raylib)
    // On ouvre une fenêtre de 800x600 pixels
    initInterface(800, 600, "Netflix Perso - Raylib Edition");

    // 3. Boucle Principale (Tourne à l'infini tant qu'on ne ferme pas)
    // WindowShouldClose détecte si tu cliques sur la croix ou appuies sur ESC
    while (!WindowShouldClose()) {

        // --- A. MISE A JOUR (Logique) ---
        // (Pour l'instant vide, on ajoutera les clics souris ici plus tard)

        // --- B. DESSIN (Affichage) ---
        BeginDrawing();

            ClearBackground(RAYWHITE); // On efface tout en blanc

            // On appelle notre fonction de dessin
            dessinerMenuPrincipal(catalogue);

        EndDrawing();
    }

    // 4. Nettoyage
    fermerInterface();          // Ferme la fenêtre Raylib
    freeCatalogue(catalogue);   // Vide la mémoire

    return 0;
}


/*

int main() {

    int etatExit = 0;                                   // Etat de sortie du programme
    char saisieUtilisateur[50];                         // Varible de saisie utilisateur
    t_catalogue catalogue = chargerBaseDeDonnees();     // Chargement du catalogue

    while(etatExit == 0) {

        system("cls");                          // Nettoie l'écran (Windows)

        afficherBaniere();

        afficherCatalogue(catalogue);

        afficherMenuAcceuil();
        scanf("%s", saisieUtilisateur);

        mettreEnMinuscule(saisieUtilisateur);

        // Verification de Exit
        if (strcmp(saisieUtilisateur, "q") == 0) {
            etatExit = 1;                           // On quitte la boucle
        }

        // Verification de Recherche
        else if (strcmp(saisieUtilisateur, "r") == 0) {
            system("cls");                          // Nettoie l'écran (Windows)
            afficherBaniere();                      // Affiche la baniere

            rechercherFilmParTitre(catalogue);      // Rechercher et afficher les films

            afficherMenuRecherche();                // Affiche le menu recherche

            int choixUserRecherche = 0;              // Initialisation du choix de l'utilisateur
            scanf("%d", &choixUserRecherche);
            
        }        

        // Verification du choix d'un Media
        else if (estNumerique(saisieUtilisateur)) {
            int choixUserMedia = convertirEnEntier(saisieUtilisateur);
            int nbTotalMedia = getNbMedia(catalogue);

            if (choixUserMedia > 0 && choixUserMedia <= nbTotalMedia) {
                system("cls");                                                      // Nettoie l'écran (Windows)
                afficherBaniere();                                                  // Affiche la baniere

                int indexMedia = choixUserMedia -1;                          // On calcule l'index du film

                afficherMedia(getMediaCatalogue(catalogue, indexMedia));            // On affiche les détails du media choisi

                afficherMenuMedia();                                                // On affiche le menu media

                int choixUserMediaLancer = 0;                                          // Initialisation du choix de l'utilisateur7

                scanf("%d", &choixUserMediaLancer);

                if (choixUserMediaLancer == 1) {
                    t_media media_a_lancer = getMediaCatalogue(catalogue, indexMedia);  // On récupère le media à lancer
                    lancerVideo(media_a_lancer);  
                } else {
                    continue;
                }
            }
        }
    }

    freeCatalogue(catalogue);    // Libération de la mémoire

    printf("[*] Fermeture...\n");

    return 0;
}

*/
