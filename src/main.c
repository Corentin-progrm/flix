#include <stdio.h>
#include <stdlib.h>
#include "modele.h"
#include "affichage.h"
#include "storage.h"

int main() {

    int choix = 1;      // a 1 pour eviter le exit direct
    t_catalogue catalogue = NULL;   // on declare le catalogue

    do
    {
        if (catalogue == NULL) {
            catalogue = chargerBaseDeDonnees();     // si le catalogue est vide, on charge la base de donnee dedans
        }

        afficherAccueil(catalogue);    // on affiche le catalogue

        choix = afficherMenuAcceuil();     // on affiche le menu en premier

        int nb_total_film = get_nb_media(catalogue);        // on recupere le nombre total de films
        int indexFilm = -1;                                 // index du film a lancer

        if (catalogue != NULL && nb_total_film > 0) {
            if (choix >= 1 && choix <= nb_total_film) {
                indexFilm = choix -1;                       // on ajuste l'index

                afficherMedia(get_media_catalogue(catalogue, indexFilm));                     // on affiche le menu media

                choix = afficherMenuMedia();                                    // on affiche le menu media

                if (choix == 1) {
                    t_media media_a_lancer = get_media_catalogue(catalogue, indexFilm);
                    lancerVideo(media_a_lancer);     // on lance la video
                } else {
                    continue;   // retour au menu accueil
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