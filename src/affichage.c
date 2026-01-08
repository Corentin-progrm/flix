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
 * @fonction afficherMenuAcceuil()
 * @brief Affiche le menu principal et retourne le choix de l'utilisateur.
 * @return int Le choix de l'utilisateur.
 */
int afficherMenuAcceuil(void) {
    
    int choix = 0;      // Initialisation du choix

    printf("\n=== Menu Acceuil ===\n");
    printf("1. Voir un media (taper le numero)\n");
    printf("0. Quitter\n");
    printf("Votre choix: ");
    
    // Sécurité de saisie (si l'utilisateur entre autre chose qu'un nombre)
    if (scanf("%d", &choix) != 1) {
        while(getchar() != '\n'); // Vider le buffer si erreur
        return 0;
    }
    return choix;
}

/**
 * @fonction afficherMenuMedia()
 * @brief Affiche le menu d'un media et retourne le choix de l'utilisateur.
 * @return int Le choix de l'utilisateur.
 */
int afficherMenuMedia(void) {

    int choix = 0;      // Initialisation du choix

    printf("\n=== Menu Media ===\n");
    printf("1. Lancer un media\n"); 
    printf("2. Retour Acceuil\n");
    printf("Votre choix: ");
    
    // Sécurité de saisie (si l'utilisateur entre autre chose qu'un nombre)
    if (scanf("%d", &choix) != 1) {
        while(getchar() != '\n'); // Vider le buffer si erreur
        return 0;
    }
    return choix;
}

/**
 * @fonction afficherAccueil()
 * @brief Affiche l'accueil du programme.
 * @param catalogue Le catalogue de médias à afficher.
 */
void afficherAccueil(t_catalogue catalogue) {

    system("cls");      // Nettoie l'écran (Windows)

    printf("============================================\n");
    printf("========= Bienvenue dans NounaFlix =========\n");
    printf("============================================\n");

    // Verification que le catalogue n'est pas vide
    if (catalogue == NULL) return;

    int nb_total_media = getNbMedia(catalogue);   // On recupere le nombre total de medias

    printf("\n============================ CATALOGUE (%d films) ============================\n", nb_total_media);
    
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
    printf("=============================================================================\n");

}

/**
 * @fonction afficherMedia()
 * @brief Affiche les détails d'un media.
 * @param  media    Le media à afficher.
 */
void afficherMedia(t_media media) {

    system("cls");  // Nettoie l'écran (Windows)

    printf("======= %s =======\n", getTitre(media));
    printf("\n");
    printf("| Code: %s\n", getCode(media));
    printf("| Type: %s\n", getType(media));
    printf("| Annee de sortie: %d\n", getAnnee(media));
    printf("| Duree: %d minutes\n", getDuree(media));
    printf("| Auteur: %s\n", getAuteur(media));

    printf("\n=============================\n");
}

