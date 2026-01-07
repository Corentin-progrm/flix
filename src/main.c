#include <stdio.h>
#include <stdlib.h>
#include "resultat.h"
#include "interface.h"

int main() {
    // Variable pour stocker notre base de données en mémoire
    t_resultat monCatalogue = NULL;
    int choix = 0;

    // Boucle principale
    do {
        // On récupère le choix UNE SEULE FOIS
        choix = afficherMenu();

        switch (choix) {
            case 1:
                // Si le catalogue est vide, on le charge
                if (monCatalogue == NULL) {
                    monCatalogue = chargerBaseDeDonnees();
                }
                // Ensuite, on l'affiche
                afficherCatalogue(monCatalogue);
                break;

            case 2:
                printf("Fermeture...\n");
                break;

            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 2);

    // NETTOYAGE FINAL
    if (monCatalogue != NULL) {
        freeResultat(monCatalogue);
    }

    return 0;
}