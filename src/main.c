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

/* DEFINITION DES FONCTIONS =============================================== */

/**
 * @fonction main()
 * @brief Point d'entrée du programme.
 * @return int Code de sortie du programme.
 */
int main() {

    int choix = 1;                      // Initialisation du choix de l'utilisateur
    t_catalogue catalogue = NULL;       // Declaration du catalogue

    // Tant que l'utilisateur ne choisit pas de quitter
    do
    {
        // Si le catalogue n'est pas encore chargé
        if (catalogue == NULL) {
            catalogue = chargerBaseDeDonnees();
        }

        afficherAccueil(catalogue);         // On affiche l'acceuil

        choix = afficherMenuAcceuil();      // On affiche le menu acceuil

        int nb_total_film = getNbMedia(catalogue);        // Nombre total de films dans le catalogue
        int indexFilm = -1;                                 // Index du film choisi

        // Si l'utilisateur a choisi un film valide
        if (catalogue != NULL && nb_total_film > 0) {

            // On vérifie que le choix est dans la plage valide
            if (choix >= 1 && choix <= nb_total_film) {
                
                indexFilm = choix -1;                                       // On calcule l'index du film

                afficherMedia(getMediaCatalogue(catalogue, indexFilm));   // On affiche les détails du media choisi

                choix = afficherMenuMedia();                                // On affiche le menu media

                // Si l'utilisateur choisit de lancer le media
                if (choix == 1) {

                    t_media media_a_lancer = getMediaCatalogue(catalogue, indexFilm); // On récupère le media à lancer
                    lancerVideo(media_a_lancer);                                        // On lance le media

                } else {
                    continue;   // Retour au menu acceuil
                }
                
            }

        }

    } while (choix != 0);

    printf("[*] Fermeture...\n");

    // On netoie avant de quitter
    if (catalogue != NULL) {
        freeCatalogue(catalogue);
    }

    return 0;
}