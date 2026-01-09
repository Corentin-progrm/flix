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
    SetTargetFPS(60);
}

void fermerInterface(void) {
    CloseWindow();
}

void dessinerFondMenu(void) {
    DrawText("NETFLIX PERSO", 20, 20, 40, RED);
    DrawLine(20, 70, 780, 70, LIGHTGRAY); // Une petite ligne de séparation
}

// --- NOTRE FONCTION MAGIQUE ---
int dessinerBouton(Rectangle rect, char* texte, Color couleurBase) {
    
    int estClique = 0;
    Vector2 souris = GetMousePosition(); // Récupère X et Y de la souris
    
    // 1. Est-ce que la souris est DANS le rectangle ?
    if (CheckCollisionPointRec(souris, rect)) {
        
        // OUI -> On change la couleur (Effet Hover)
        DrawRectangleRec(rect, SKYBLUE); 
        
        // 2. Est-ce qu'on a cliqué ?
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            estClique = 1;
        }
    } else {
        // NON -> Couleur normale
        DrawRectangleRec(rect, couleurBase);
    }

    // 3. On dessine le contour et le texte
    DrawRectangleLinesEx(rect, 2, DARKGRAY); // Bordure
    
    // Centrage approximatif du texte
    int largeurTexte = MeasureText(texte, 20);
    int posX = rect.x + (rect.width - largeurTexte) / 2;
    int posY = rect.y + (rect.height - 20) / 2;
    
    DrawText(texte, posX, posY, 20, BLACK);

    return estClique;
}