#include <stdio.h>
#include <stdlib.h>

#include "storage.h"


int fichierExiste(char *chemin) {
    FILE *f = fopen(chemin, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}



// Extrait de src/interface.c à modifier
t_catalogue chargerBaseDeDonnees(void) {
    // 1. Création de la structure
    t_catalogue catalogue = creer_catalogue();
    int max_taille = 100; // Limite arbitraire

    // 2. Allocation de la mémoire
    allouerTableauMedia(catalogue, max_taille);
    // 3. Ouverture du fichier
    FILE *fichier = fopen("assets/base_de_donnees.txt", "r");
    if (!fichier) {
        printf("[ERREUR] Fichier 'assets/base_de_donnees.txt' introuvable.\n");
        return catalogue; // On retourne une structure vide mais valide
    }

    char ligne[256];
    int compteur = 0;
    char cheminVideo[100];

    printf("[*] Chargement du catalogue en cours...\n");

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
                setMediaCatalogue(catalogue, m, compteur);
                compteur++;
                
            } else {
                // On garde quand même ce message, c'est utile pour l'utilisateur
                printf("[*] Video manquante pour : %s (Ignore)\n", tTitre);
            }
        }
    }

    // 5. Finalisation
    setNbMedia(catalogue, compteur);
    fclose(fichier);
    
    return catalogue;
}

void lancerVideo(t_media media) {
    if (media == NULL) return;

    char commande[256];
    
    sprintf(commande, "start assets/%s.mp4", getCode(media));
    
    printf("[*] Lancement de : %s ...\n", getTitre(media));
    
    system(commande);
}