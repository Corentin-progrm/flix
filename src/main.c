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
    t_catalogue catalogue = chargerBaseDeDonnees();
    initInterface(800, 600, "Netflix Perso - Mouse Edition");

    while (!WindowShouldClose()) {

        // --- DESSIN & LOGIQUE EN MÊME TEMPS ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // 1. Le titre et le fond
            dessinerFondMenu();

            // 2. Bouton QUITTER (En haut à droite)
            // Rectangle : x=650, y=20, largeur=130, hauteur=40
            Rectangle rectQuitter = {650, 20, 130, 40};
            if (dessinerBouton(rectQuitter, "QUITTER", LIGHTGRAY)) {
                break; // Sort de la boucle -> Ferme le programme
            }

            // 3. Bouton RECHERCHER (À côté)
            Rectangle rectSearch = {500, 20, 130, 40};
            if (dessinerBouton(rectSearch, "RECHERCHE", LIGHTGRAY)) {
                // Pour l'instant on met juste un message console, 
                // on fera la fenêtre de recherche plus tard
                printf("Clic sur Recherche !\n");
            }

            // 4. LISTE DES FILMS CLIQUABLE
            int y = 100;
            int nbFilms = getNbMedia(catalogue);

            for (int i = 0; i < nbFilms; i++) {
                t_media m = getMediaCatalogue(catalogue, i);
                
                // On crée un rectangle pour chaque ligne de film
                // Il prend toute la largeur de l'écran
                Rectangle rectFilm = {20, y, 760, 30};
                
                // Astuce : On utilise notre fonction bouton mais avec une couleur transparente (BLANK)
                // ou un gris très clair pour faire joli.
                // Le texte du bouton sera le titre du film.
                char titreComplet[100];
                sprintf(titreComplet, "%s (%d)", getTitre(m), getAnnee(m));

                // Si on clique sur le film...
                if (dessinerBouton(rectFilm, titreComplet, RAYWHITE)) {
                    printf("Lancement de : %s\n", getTitre(m));
                    lancerVideo(m);
                }

                y += 35; // Espace entre les lignes
            }

        EndDrawing();
    }

    fermerInterface();
    freeCatalogue(catalogue);
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
