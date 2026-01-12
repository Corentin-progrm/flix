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
#define CHEMIN_MEDIAS "assets/medias/%s.mp4"
#define CHEMIN_LANCEMENT_MEDIAS "start assets/medias/%s.mp4"

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
 * @fonction lancerVideo()
 * @brief Lance la vidéo associée au media.
 * @param  media    Le media dont on veut lancer la vidéo.
 */
void lancerVideo(t_media media) {

    if (media == NULL) return;  // Verification media valide

    char commande[256];         // Buffer pour la commande système
    
    sprintf(commande, CHEMIN_LANCEMENT_MEDIAS, getCode(media));   //
    
    printf("[*] Lancement de : %s ...\n", getTitre(media));
    
    system(commande);
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
            sprintf(cheminVideo, CHEMIN_MEDIAS, tCode);
            
            // On vérifie si la vidéo existe réellement
            if (fichierExiste(cheminVideo)) {
                
                // Création et remplissage du media
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
                printf("[ERREUR] Video manquante pour : %s (Ignore)\n", tTitre); // Log si video manquante
            }
        }
    }

    // On met à jour le nombre de medias dans le catalogue
    setNbMedia(catalogue, compteur);
    fclose(fichier);
    
    return catalogue;
}


int mediaCorrespondCategorie(t_media m, int indexMenu) {
    
    // Si aucun filtre n'est actif (-1) ou si on clique sur Search(1) ou Ajouter(0),
    // on décide d'afficher TOUT par défaut (ou on gère autrement).
    // Ici, disons que si on n'a pas cliqué sur une catégorie précise, on montre tout.
    if (indexMenu == -1 || indexMenu == 0 || indexMenu == 1) return 1;

    // Récupération du type de média (Assure-toi d'avoir cette info dans ta struct)
    // Sinon, remplace getGenre(m) par le champ approprié
    char* typeMedia = getType(m); 

    // MAPPING DES BOUTONS (Selon l'ordre dans ta fonction dessinerBarreCategories)
    // 2 = "Film"
    if (indexMenu == 2) {
        if (strcmp(typeMedia, "Film") == 0) return 1;
    }
    // 3 = "Serie"
    else if (indexMenu == 3) {
        if (strcmp(typeMedia, "Serie") == 0) return 1;
    }
    // 4 = "Autre"
    else if (indexMenu == 4) {
        // Affiche si ce n'est ni Film ni Série, ou si c'est explicitement "Autre"
        if (strcmp(typeMedia, "Film") != 0 && strcmp(typeMedia, "Serie") != 0) return 1;
    }

    return 0; // Ne correspond pas
}


int texteContient(const char* haystack, const char* needle) {
    if (!needle || strlen(needle) == 0) return 1; // Recherche vide = tout matche

    // On fait des copies locales pour ne pas modifier les vraies données
    char h[100];
    char n[100];
    
    // Copie et conversion en minuscule
    strncpy(h, haystack, 99); h[99]='\0';
    strncpy(n, needle, 99); n[99]='\0';
    
    for(int i = 0; h[i]; i++) h[i] = tolower(h[i]);
    for(int i = 0; n[i]; i++) n[i] = tolower(n[i]);

    // Comparaison standard
    return (strstr(h, n) != NULL);
}

// Fonction appelée par l'affichage pour filtrer
int mediaCorrespondRecherche(t_media m, char* texteRecherche) {
    // Si la recherche est vide, on affiche tout
    if (texteRecherche == NULL || strlen(texteRecherche) == 0) return 1;

    // On cherche dans le TITRE
    return texteContient(getTitre(m), texteRecherche);
}

