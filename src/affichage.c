#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "affichage.h"


// Affichage du menu
int afficherMenuAcceuil(void) {
    int choix = 0;
    printf("\n=== Menu Acceuil ===\n");
    printf("1. Voir un media (taper le numero)\n"); // On charge ET on affiche
    printf("0. Quitter\n");
    printf("Votre choix: ");
    
    // Sécurité de saisie
    if (scanf("%d", &choix) != 1) {
        while(getchar() != '\n'); // Vider le buffer si erreur
        return 0;
    }
    return choix;
}

// Affichage du menu
int afficherMenuMedia(void) {
    int choix = 0;
    printf("\n=== Menu Media ===\n");
    printf("1. Lancer un media\n"); // On charge ET on affiche
    printf("2. Retour Acceuil\n");
    printf("Votre choix: ");
    
    // Sécurité de saisie
    if (scanf("%d", &choix) != 1) {
        while(getchar() != '\n'); // Vider le buffer si erreur
        return 0;
    }
    return choix;
}

// Affiche l'acceuil du programme
void afficherAccueil(t_catalogue catalogue) {

    system("cls");

    printf("=============================================\n");
    printf("=== Bienvenue dans NounaFlix ===\n");
    printf("=============================================\n\n");

    // Verification que le catalogue n'est pas vide
    if (catalogue == NULL) return;

    int nb_total_media = get_nb_media(catalogue);

    printf("\n============================ CATALOGUE (%d films) ============================\n", nb_total_media);
    
    for (int i = 0; i < nb_total_media; i++) {
        // On récupère le film numéro i
        t_media m = get_media_catalogue(catalogue, i);

        // On affiche ses infos via les getters
        printf("| [%d] | %-8s, %4d\n", 
            i+1,
            getTitre(m), 
            getAnnee(m));
                
    }
    printf("=============================================================================\n");

}

// Page detaillee d'un media
void afficherMedia(t_media media) {

    system("cls");

    printf("======= %s =======\n", getTitre(media));
    printf("\n");
    printf("| Code: %s\n", getCode(media));
    printf("| Type: %s\n", getType(media));
    printf("| Annee de sortie: %d\n", getAnnee(media));
    printf("| Duree: %.0f minutes\n", getDuree(media));
    printf("| Auteur: %s\n", getAuteur(media));

    printf("\n=============================\n");
}

