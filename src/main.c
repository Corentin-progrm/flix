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

/**
 * Module main - main.c
 * Par : Corentin Couëron
 * Description : Main complet avec Navigation (Accueil/Détail) et Recherche fonctionnelle.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Pour strlen
#include "modele.h"
#include "affichage.h"
#include "storage.h"

int main(void)
{
    initInterface(1024, 768, "NounaFlix");
    
    // Animation (Tu peux la décommenter quand tu veux)
    animLogoStart();

    t_catalogue catalogue = chargerBaseDeDonnees();
    chargerTexturesCatalogue(catalogue);

    // --- ETATS DE L'APPLICATION ---
    int etatApp = 0;          // 0 = ACCUEIL, 1 = DETAILS
    int indexFilmChoisi = -1; // Le film qu'on regarde en détails

    // --- ETATS DES FILTRES ---
    int filtreSelectionne = -1;       // -1 = Tout, 0=Ajout, 1=Search...
    char bufferRecherche[64] = { 0 }; // Texte de recherche
    int modeRecherche = 0;            // 0 = Caché, 1 = Visible

    while (!WindowShouldClose()) 
    {
        // ============================================================
        // 1. GESTION DU CLAVIER (C'est ça qui te manquait !)
        // ============================================================
        if (modeRecherche && etatApp == 0) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (strlen(bufferRecherche) < 63)) {
                    int len = strlen(bufferRecherche);
                    bufferRecherche[len] = (char)key;
                    bufferRecherche[len + 1] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = strlen(bufferRecherche);
                if (len > 0) bufferRecherche[len - 1] = '\0';
            }
        }

        // ============================================================
        // 2. DESSIN ET LOGIQUE GRAPHIQUE
        // ============================================================
        BeginDrawing();
            
            // Fond commun
            ClearBackground(GetColor(0x141414FF)); 
            dessinerEnTete();

            // --- CAS 0 : PAGE D'ACCUEIL ---
            if (etatApp == 0) {
                
                // A. Menu Catégories
                int clicMenu = dessinerBarreCategories();
                
                if (clicMenu != -1) {
                    if (clicMenu == 1) {
                        // Clic sur SEARCH : On active/désactive la barre
                        modeRecherche = !modeRecherche;
                        if (modeRecherche) filtreSelectionne = -1; // Reset filtre
                    } else {
                        // Clic sur une CATEGORIE
                        if (filtreSelectionne == clicMenu) filtreSelectionne = -1;
                        else filtreSelectionne = clicMenu;
                        
                        // Optionnel : Désactiver la recherche textuelle si on change de catégorie
                        // modeRecherche = 0; 
                    }
                }

                // B. Barre de recherche (si active)
                if (modeRecherche) {
                    // On passe bufferRecherche à la fonction d'affichage
                    // (Assure-toi que cette fonction est bien dans affichage.c/.h)
                    // Si tu ne l'as pas encore ajoutée au .h, copie le prototype !
                    dessinerBarreRecherche(bufferRecherche, 1);
                }

                // C. La Grille des Films
                // On utilise la boucle manuelle ici pour gérer le changement d'état (clic -> détails)
                
                int nbFilms = getNbMedia(catalogue);
                int startX = 40; 
                int startY = 220; // Assez bas pour laisser la place au menu/recherche
                int largeurFenetre = GetScreenWidth();
                int espaceTotalCarte = CARTE_LARGEUR + CARTE_PADDING;
                int colonnesMax = (largeurFenetre - (startX * 2)) / espaceTotalCarte;
                if (colonnesMax < 1) colonnesMax = 1;

                int compteurAffiches = 0;
                
                for (int i = 0; i < nbFilms; i++) {
                    t_media m = getMediaCatalogue(catalogue, i);

                    // LOGIQUE DE FILTRAGE (Mixte : Catégorie + Texte)
                    // Note : mediaCorrespondCategorie doit etre dans ton modele.h
                    // Note : mediaCorrespondRecherche doit etre dans ton modele.h
                    int matchCat = mediaCorrespondCategorie(m, filtreSelectionne);
                    int matchTexte = mediaCorrespondRecherche(m, bufferRecherche);

                    if (matchCat && matchTexte) {
                        
                        int colonne = compteurAffiches % colonnesMax;
                        int ligne = compteurAffiches / colonnesMax;
                        Rectangle rect = { 
                            (float)(startX + colonne * espaceTotalCarte), 
                            (float)(startY + ligne * (CARTE_HAUTEUR + CARTE_PADDING)), 
                            (float)CARTE_LARGEUR, (float)CARTE_HAUTEUR 
                        };

                        if (mesTextures != NULL) {
                            // Si on clique sur une carte...
                            if (dessinerCarteMedia(rect, m, mesTextures[i])) {
                                indexFilmChoisi = i; // On mémorise quel film
                                etatApp = 1;         // ON PASSE A LA PAGE DETAILS
                            }
                        }
                        compteurAffiches++;
                    }
                }
            }

            // --- CAS 1 : PAGE DETAILS ---
            else if (etatApp == 1) {
                if (indexFilmChoisi >= 0) {
                    t_media m = getMediaCatalogue(catalogue, indexFilmChoisi);
                    
                    // Affiche la fiche et récupère l'action (1=Retour, 2=Play)
                    int action = dessinerPageDetails(m, mesTextures[indexFilmChoisi]);

                    if (action == 1) etatApp = 0; // Retour Accueil
                    else if (action == 2) lancerVideo(m); // Lecture
                }
            }

        EndDrawing();
    }

    libererTexturesCatalogue();
    freeCatalogue(catalogue);
    fermerInterface();

    return 0;
}