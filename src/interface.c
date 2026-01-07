#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

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

int fichierExiste(char *chemin) {
    FILE *f = fopen(chemin, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}

// Nouvelle fonction d'affichage qui prend la structure en paramètre
void afficherCatalogue(t_resultat resultat) {
    if (resultat == NULL) return;

    int nb = get_nb_media(resultat);
    printf("\n============================ CATALOGUE (%d films) ============================\n", nb);
    
    for (int i = 0; i < nb; i++) {
        // On récupère le film numéro i
        t_media m = get_media_resultat(resultat, i);
        
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

// Extrait de src/interface.c à modifier
t_resultat chargerBaseDeDonnees(void) {
    // 1. Création de la structure
    t_resultat resultat = creer_resultat();
    int max_taille = 100; // Limite arbitraire

    // 2. Allocation de la mémoire
    allouerTableauMedia(resultat, max_taille);

    // 3. Ouverture du fichier
    FILE *fichier = fopen("assets/base_de_donnees.txt", "r");
    if (!fichier) {
        printf("[ERREUR] Fichier 'assets/base_de_donnees.txt' introuvable.\n");
        return resultat; // On retourne une structure vide mais valide
    }

    char ligne[256];
    int compteur = 0;
    char cheminVideo[100];

    printf("Chargement du catalogue en cours...\n");

    // 4. Lecture ligne par ligne
    while (compteur < max_taille && fgets(ligne, sizeof(ligne), fichier)) {
        
        char tCode[50], tType[50], tTitre[100], tAuteur[100];
        int tAnnee; 
        float tDuree;

        // Parsing (Découpage de la ligne)
        int n = sscanf(ligne, "%[^;];%[^;];%[^;];%d;%f;%[^\n]", 
                       tCode, tType, tTitre, &tAnnee, &tDuree, tAuteur);

        // Si la ligne est bien formée (6 éléments trouvés)
        if (n == 6) {
            sprintf(cheminVideo, "assets/%s.mp4", tCode);
            
            // On vérifie si la vidéo existe réellement
            if (fichierExiste(cheminVideo)) {
                
                t_media m = creer_media();
                setCode(m, tCode);
                setType(m, tType);
                setTitre(m, tTitre);
                setAnnee(m, tAnnee);
                setDuree(m, tDuree);
                setAuteur(m, tAuteur);
                
                // On ajoute au catalogue
                setMediaResultat(resultat, m, compteur);
                compteur++;
                
            } else {
                // On garde quand même ce message, c'est utile pour l'utilisateur
                printf(" -> [INFO] Video manquante pour : %s (Ignore)\n", tTitre);
            }
        }
    }

    // 5. Finalisation
    setNbMedia(resultat, compteur);
    fclose(fichier);
    
    printf("Termine : %d films charges avec succes.\n", compteur);
    return resultat;
}