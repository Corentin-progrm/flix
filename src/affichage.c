#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "affichage.h"

// Affichage du menu
int afficherMenu(void) {
    int choix = 0;
    printf("\n=== Menu Principal ===\n");
    printf("1. Charger et Afficher le catalogue\n"); // On charge ET on affiche
    printf("2. Quitter\n");
    printf("Votre choix: ");
    
    // Sécurité de saisie
    if (scanf("%d", &choix) != 1) {
        while(getchar() != '\n'); // Vider le buffer si erreur
        return 0;
    }
    return choix;
}



// Nouvelle fonction d'affichage qui prend la structure en paramètre
void afficherCatalogue(t_catalogue catalogue) {
    if (catalogue == NULL) return;

    int nb = get_nb_media(catalogue);
    printf("\n============================ CATALOGUE (%d films) ============================\n", nb);
    
    for (int i = 0; i < nb; i++) {
        // On récupère le film numéro i
        t_media m = get_media_catalogue(catalogue, i);
        
        // On affiche ses infos via les getters
        printf("| %-8s | %-20s | %4d | %-10s | %.0f min\n", 
               getCode(m), 
               getTitre(m), 
               getAnnee(m), 
               getAuteur(m),
               getDuree(m));
    }
    printf("==============================================================================\n");
}

