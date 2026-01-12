/**
 * Module main - main.c
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module main est le point d'entrée du programme.
**/

/* LIBRARY ================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include "modele.h"
#include "affichage.h"
#include "storage.h"
#include "utils.h"

/* DEFINITION DES FONCTIONS =============================================== */

int main(void)
{
    initInterface(1024, 768, "NounaFlix");
    animLogoStart();

    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue);

    // --- ETATS DE L'APPLICATION ---
    int etatApp = 0;        // 0 = ACCUEIL (Grille), 1 = DETAILS (Fiche Film)
    int indexFilmChoisi = -1; // Pour savoir quel film afficher en détails

    // Filtres (comme avant)
    int filtreSelectionne = -1;
    char bufferRecherche[64] = { 0 };
    int modeRecherche = 0;

    while (!WindowShouldClose()) 
    {
        // ... Gestion clavier (Copie ton code clavier ici si tu veux la recherche) ...
        
        BeginDrawing();
            
            // Fond et En-tête communs aux deux pages
            ClearBackground(GetColor(0x141414FF)); 
            dessinerEnTete();

            // =========================================================
            // CAS 1 : PAGE D'ACCUEIL (La Grille)
            // =========================================================
            if (etatApp == 0) {
                
                // 1. Menu et Recherche
                int clicMenu = dessinerBarreCategories();
                // ... (Ta logique de menu ici) ...
                if (clicMenu != -1) {
                     if (clicMenu == 1) modeRecherche = !modeRecherche;
                     else filtreSelectionne = (filtreSelectionne == clicMenu) ? -1 : clicMenu;
                }
                if (modeRecherche) dessinerBarreRecherche(bufferRecherche, 1);

                // 2. Grille (Modifiée pour ne pas lancer la vidéo mais CHANGER D'ETAT)
                // J'ai recopié ta logique de grille mais changé l'action du clic
                
                int nbFilms = getNbMedia(catalogue);
                int startX = 40; int startY = 220;
                int largeurFenetre = GetScreenWidth();
                int espaceTotalCarte = CARTE_LARGEUR + CARTE_PADDING;
                int colonnesMax = (largeurFenetre - (startX * 2)) / espaceTotalCarte;
                if (colonnesMax < 1) colonnesMax = 1;

                int compteurAffiches = 0;
                for (int i = 0; i < nbFilms; i++) {
                    t_media m = getMediaCatalogue(catalogue, i);
                    
                    // Filtrage (Ta fonction existe déjà)
                    // (Simplifié ici pour l'exemple, utilise ta fonction dessinerGrilleFiltree c'est mieux)
                    // Mais attention : dessinerGrilleFiltree doit maintenant renvoyer l'index cliqué !
                    
                    // Pour faire simple dans le main directement :
                    int match = mediaCorrespondCategorie(m, filtreSelectionne) && mediaCorrespondRecherche(m, bufferRecherche);
                    
                    if (match) {
                        int colonne = compteurAffiches % colonnesMax;
                        int ligne = compteurAffiches / colonnesMax;
                        Rectangle rect = { (float)(startX + colonne * espaceTotalCarte), (float)(startY + ligne * (CARTE_HAUTEUR + CARTE_PADDING)), (float)CARTE_LARGEUR, (float)CARTE_HAUTEUR };

                        if (mesTextures != NULL) {
                            if (dessinerCarteMedia(rect, m, mesTextures[i])) {
                                
                                // CLIC SUR UN FILM : ON CHANGE D'ETAT !
                                indexFilmChoisi = i; // On retient lequel
                                etatApp = 1;         // On va vers la page DETAILS
                            
                            }
                        }
                        compteurAffiches++;
                    }
                }
            }

            // =========================================================
            // CAS 2 : PAGE DETAILS (Le Film seul)
            // =========================================================
            else if (etatApp == 1) {
                
                // On récupère le média sélectionné
                t_media m = getMediaCatalogue(catalogue, indexFilmChoisi);
                
                // On affiche la page et on récupère l'action de l'utilisateur
                // (Note : mesTextures[indexFilmChoisi] est l'affiche du film)
                int action = dessinerPageDetails(m, mesTextures[indexFilmChoisi]);

                if (action == 1) {
                    // Clic sur RETOUR
                    etatApp = 0; // On revient à la grille
                }
                else if (action == 2) {
                    // Clic sur LECTURE
                    printf("Lancement video...\n");
                    lancerVideo(m);
                }
            }

        EndDrawing();
    }

    libererTexturesCatalogue();
    freeCatalogue(catalogue);
    fermerInterface();

    return 0;
}