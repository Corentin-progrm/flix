/**
 * Module affichage - affichage.c
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module affichage gère toutes les fonctions
 *               d'affichage du programme, y compris les menus
 *              et les détails des medias.
**/

/* LIBRARY ================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "affichage.h"

/* DEFINITION DES FONCTIONS =============================================== */

/**
 * @fonction afficherTitre()
 * @brief Affiche le titre du programme.
 */
int afficherBaniere(void) {
    printf("============================================\n");
    printf("========= Bienvenue dans NounaFlix =========\n");
    printf("============================================\n");

    return 0;
}

int afficherMenuAcceuil(void) {
    printf("\n=============== MENU ACCEUIL ===============\n");
    printf("[1,2,3...] Voir un media\n");
    printf("[R] Rechercher un media\n");
    printf("[Q] Quitter l'application\n");
    printf("============================================\n");
    printf("> ");

    return 0;
}

int afficherMenuMedia(void) {
    printf("\n=============== MENU MEDIA ===============\n");
    printf("[1] Lancer le media\n"); 
    printf("[Q] Retour Catalogue");
    printf("\n==========================================\n");
    printf("> ");

    return 0;
}

int afficherMenuRecherche(void) {
    printf("\n=============== MENU RECHERCHE ===============\n");
    printf("[1,2,3...] Voir un media\n");
    printf("[Q] Retour Acceuil\n");
    printf("================================================\n");
    printf("> ");

    return 0;
}

/**
 * @fonction afficherAccueil()
 * @brief Affiche l'accueil du programme.
 * @param catalogue Le catalogue de médias à afficher.
 */
int afficherCatalogue(t_catalogue catalogue) {

    if (catalogue == NULL) return 0;                  // Verification que le catalogue n'est pas vide
    int nb_total_media = getNbMedia(catalogue);     // On recupere le nombre total de medias

    printf("\n=========== CATALOGUE (%d films) ===========\n", nb_total_media);
    
    // Pour tous les medias du catalogue
    for (int i = 0; i < nb_total_media; i++) {
        // On récupère le film numéro i
        t_media m = getMediaCatalogue(catalogue, i);

        // On affiche ses infos via les getters
        printf("| [%d] | %-8s, %4d\n", 
            i+1,
            getTitre(m), 
            getAnnee(m));        
    }
    printf("===========================================\n");

    return 0;
}

/**
 * @fonction afficherMedia()
 * @brief Affiche les détails d'un media.
 * @param  media    Le media à afficher.
 */
void afficherMedia(t_media media) {

    printf("\n======= %s =======\n", getTitre(media));
    printf("| Code: %s\n", getCode(media));
    printf("| Type: %s\n", getType(media));
    printf("| Annee de sortie: %d\n", getAnnee(media));
    printf("| Duree: %d minutes\n", getDuree(media));
    printf("| Auteur: %s\n", getAuteur(media));
    printf("=============================\n");
}

void rechercherFilmParTitre(t_catalogue catalogue) {
    
    if (catalogue == NULL) return;                  // Verification que le catalogue est pas vide

    char recherche[100];                            // Buffer pour la recherche
    int nb_trouve = 0;                              // Compteur de films trouvés

    printf("\n=============== RECHERCHE ===============\n");
    printf("> ");
    
    if (scanf(" %[^\n]", recherche) != 1) return;   // On lit la chaine de caractere

    printf("\nResultats pour \"%s\" :\n", recherche);
    printf("\n============ \"%s\" Resultats ============\n", recherche);

    int nb_total = getNbMedia(catalogue);         // Nombre total de media dans le catalogue

    // On parcours tout le catalogue
    for (int i = 0; i < nb_total; i++) {

        t_media m = getMediaCatalogue(catalogue, i);  // On recupere le media i

        char* titreActuel = getTitre(m);                // On recupere son titre

        // Si le titre contient la chaine recherchée
        if (strstr(titreActuel, recherche) != NULL) {
            
            printf("| [%d] | %-8s, %4d\n", 
                    i+1,
                    getTitre(m), 
                    getAnnee(m));
            
            nb_trouve++;
        }
    }
    printf("\n=========================================\n");

    if (nb_trouve == 0) {
        printf("Aucun film trouve contenant \"%s\".\n", recherche);
    } else {
        printf("%d film(s) trouve(s).\n", nb_trouve);
    }
}

/* INTERFACE GRAPHIQUE ==================================================== */

void initInterface(int largeur, int hauteur, char* titre) {
    InitWindow(largeur, hauteur, titre);
    SetTargetFPS(60); // On bloque à 60 images par seconde pour que ce soit fluide
}

void fermerInterface(void) {
    CloseWindow();
}

void dessinerMenuPrincipal(t_catalogue catalogue) {
    // 1. On dessine le titre de l'application
    DrawText("NETFLIX PERSO", 20, 20, 40, RED);
    DrawText("Appuyez sur ESC pour quitter", 20, 70, 20, DARKGRAY);

    // 2. On dessine la liste des films
    int y = 120; // Position verticale de départ (pixels)
    int x = 20;  // Position horizontale

    int nbFilms = getNbMedia(catalogue);

    for (int i = 0; i < nbFilms; i++) {
        t_media m = getMediaCatalogue(catalogue, i);

        // On prépare le texte (ex: "[1] Inception (2010)")
        char texteFilm[100];
        sprintf(texteFilm, "[%d] %s (%d)", i + 1, getTitre(m), getAnnee(m));

        // Affichage du texte
        // Syntaxe : DrawText(texte, posX, posY, taillePolice, Couleur)
        DrawText(texteFilm, x, y, 20, BLACK);

        // On descend de 30 pixels pour le prochain film
        y += 30;
    }
}
