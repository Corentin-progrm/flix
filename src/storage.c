/**
 * Module storage - storage.c
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module storage gère le chargement de la base de données
 *               depuis un fichier texte, la vérification de l'existence des 
 *               fichiers multimédia et la lecture des informations.
**/

/* LIBRARY ================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

/* CONSTANTES ============================================================= */
#define CHEMIN_BDD "assets/base_de_donnees.txt"

/* DEFINITION DES FONCTIONS =============================================== */

/**
 * @fonction fichierExiste()
 * @brief Permet de vérifier si un fichier existe à un chemin donné.
 * @param  chemin    Le chemin du fichier à vérifier.
 * @return int       1 si le fichier existe, 0 sinon.
 */
int fichierExiste(char *chemin) {
    FILE *f = fopen(chemin, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}

/**
 * @fonction chargerBaseDeDonnees()
 * @brief Charge le fichier texte, vérifie les .mp4, et retourne TOUTE la structure remplie
 * @return t_catalogue Le catalogue chargé
 */
t_catalogue chargerBaseDeDonnees(void) {

    t_catalogue catalogue = creer_catalogue();      // Creation du catalogue vide
    int max_taille = 100;                           // Taille maximale du catalogue (arbitraire)

    allouerTableauMedia(catalogue, max_taille);     // Allocation du tableau de medias

    FILE *fichier = fopen(CHEMIN_BDD, "r");         // Ouverture du fichier de base de données
    if (!fichier) {
        printf("[ERREUR] Fichier '%s' introuvable.\n", CHEMIN_BDD);
        return catalogue;                           // Retourne un catalogue vide
    }

    char ligne[256];            // Buffer pour lire chaque ligne
    int compteur = 0;           // Compteur de medias chargés
    char cheminVideo[100];      // Buffer pour le chemin de la vidéo

    printf("[*] Chargement du catalogue en cours...\n");

    // Lecture ligne par ligne du fichier
    while (compteur < max_taille && fgets(ligne, sizeof(ligne), fichier)) {
        
        char tCode[50], tType[50], tTitre[100], tAuteur[100];
        int tAnnee; 
        int tDuree;

        // Parsing (Découpage de la ligne)
        int n = sscanf(ligne, "%[^;];%[^;];%[^;];%d;%d;%[^\n]", 
                       tCode, tType, tTitre, &tAnnee, &tDuree, tAuteur);

        // Si la ligne est bien formée (6 éléments trouvés)
        if (n == 6) {
            // Construction du chemin de la vidéo
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