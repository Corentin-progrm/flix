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

/* CONSTANTES ============================================================= */
// Couleurs de l'application
const Color THEME_FOND_APP        = { 20, 20, 20, 255 };    // Gris très foncé (Presque noir)
const Color THEME_TITRE           = { 229, 9, 20, 255 };    // Rouge "Netflix"
const Color THEME_LOGO_CADRE      = { 200, 200, 200, 255 }; // Gris clair

// Couleurs des Cartes Films
const Color CARTE_FOND            = { 40, 40, 40, 255 };    // Gris foncé
const Color CARTE_TEXTE_TITRE     = { 34, 139, 34, 255 };   // Vert forêt (Ton choix actuel)
const Color CARTE_TEXTE_ANNEE     = { 150, 150, 150, 255 }; // Gris moyen
const Color CARTE_BORDURE_REPOS   = { 0, 100, 0, 255 };     // Vert foncé
const Color CARTE_BORDURE_HOVER   = { 230, 230, 230, 255 }; // Blanc/Gris au survol
const int   CARTE_EPAISSEUR_BORD  = 3;

#define COL_BTN_AJOUTER    LIGHTGRAY
#define COL_BTN_SEARCH     LIME        // "LIME" est le vert vif standard Raylib
#define COL_BTN_FILM       MAROON      // Rouge foncé
#define COL_BTN_SERIE      PURPLE
#define COL_BTN_AUTRE      GOLD

#define CHEMIN_IMAGES "assets/images/%s.jpg" // Attention: .jpg ou .png selon tes fichiers

/* VARIABLES GLOBALES ===================================================== */
/* VARIABLES GLOBALES ===================================================== */
Texture2D* mesTextures = NULL;          
static int nbTexturesChargees = 0;
static float scrollY = 0.0f;    

// Variables de Scroll Horizontal (Une par section)
static float scrollX_Top5 = 0.0f; 
static float scrollX_Series = 0.0f; 
static float scrollX_Last = 0.0f; 

// Variables de Sélection (Les indices des films à afficher)
static int indicesTop5[5] = {-1, -1, -1, -1, -1};
static int indicesSeries[5] = {-1, -1, -1, -1, -1};
static int indicesLast[5] = {-1, -1, -1, -1, -1};

static int estSectionsInit = 0;

/* FONCTIONS UTILITAIRES ================================================== */

// Redimensionne une image pour remplir un rectangle (Mode COVER) avec découpage
static void redimensionTextureMedia(Texture2D texture, Rectangle destRect) {
    if (texture.id <= 0) return;

    // Calcul du ratio pour que l'image couvre tout le rectangle
    float scaleX = destRect.width / (float)texture.width;
    float scaleY = destRect.height / (float)texture.height;
    float scale = (scaleX > scaleY) ? scaleX : scaleY; // On prend le max pour remplir

    float texWidthZoom = texture.width * scale;
    float texHeightZoom = texture.height * scale;

    // Centrage
    Vector2 pos = { 
        destRect.x + (destRect.width - texWidthZoom) / 2.0f, 
        destRect.y + (destRect.height - texHeightZoom) / 2.0f 
    };

    // Découpage de ce qui dépasse
    BeginScissorMode((int)destRect.x, (int)destRect.y, (int)destRect.width, (int)destRect.height);
        DrawTextureEx(texture, pos, 0.0f, scale, WHITE);
    EndScissorMode();
}

// Dessine un bouton carré style "Tuile"
static int dessinerCarreMenu(Rectangle rect, char* texte, Color couleurPrincipale) {
    int estClique = 0;
    Vector2 souris = GetMousePosition();
    
    // Par défaut : Fond transparent (30%)
    Color couleurFond = Fade(couleurPrincipale, 0.3f); 
    
    // Détection Souris
    if (CheckCollisionPointRec(souris, rect)) {
        couleurFond = Fade(couleurPrincipale, 0.6f); // Plus opaque au survol (60%)
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) estClique = 1;
    }

    // Dessin
    DrawRectangleRec(rect, couleurFond);               
    DrawRectangleLinesEx(rect, 4, couleurPrincipale);  // Bordure épaisse couleur pure

    // Texte aligné en bas à droite
    int taillePolice = 20;
    int largeurTexte = MeasureText(texte, taillePolice);
    int posX = (int)(rect.x + rect.width - largeurTexte - 10);
    int posY = (int)(rect.y + rect.height - taillePolice - 10);

    DrawText(texte, posX, posY, taillePolice, RAYWHITE); // Texte en blanc pour lisibilité

    return estClique;
}

// --- HELPER : Gestion des Entrées ---
static void gererInputsScroll(Rectangle zoneTop5, int afficherTop5, int nbFilms, int xAlign) {
    float wheel = GetMouseWheelMove();
    
    // 1. Calcul des limites du scroll horizontal
    int nbTop = (nbFilms < 5) ? nbFilms : 5;
    // Largeur physique des cartes + marges (+50 pour bien fermer le cadre)
    int largeurContenuTop5 = nbTop * (CARTE_LARGEUR + 15) + 100; 
    
    // La limite prend en compte le décalage d'alignement initial (xAlign)
    int maxScrollX = (largeurContenuTop5 + xAlign) - (int)zoneTop5.width;
    
    // Sécurité : si le contenu est plus petit que l'écran, pas de scroll possible
    if (maxScrollX < 0) maxScrollX = 0;

    // IMPORTANT : Si on redimensionne la fenêtre et que tout rentre, on force le retour à 0
    if (maxScrollX == 0) {
        scrollX_Top5 = 0;
    }

    // Si pas de mouvement de souris, on s'arrête là (après avoir fait les vérifs de taille ci-dessus)
    if (wheel == 0) return;

    int sourisSurTop5 = afficherTop5 && CheckCollisionPointRec(GetMousePosition(), zoneTop5);

    if (sourisSurTop5) {
        // CAS 1 : Scroll HORIZONTAL (Souris sur le cadre vert)
        scrollX_Top5 -= wheel * 20.0f; 
        
        // Bornage STRICT (Clamping)
        // On empêche de dépasser 0 (gauche) et maxScrollX (droite)
        // Cela fonctionne même si maxScrollX vaut 0 (ça bloque le scroll sur 0)
        if (scrollX_Top5 < 0) scrollX_Top5 = 0;
        if (scrollX_Top5 > maxScrollX) scrollX_Top5 = maxScrollX;
        
    } else {
        // CAS 2 : Scroll VERTICAL (Souris ailleurs)
        scrollY -= wheel * 20.0f; 
    }
}

// Retourne 1 si un scroll horizontal a eu lieu, 0 sinon
static int gererScrollHorizontal(Rectangle zone, float* scrollVar, int nbFilms, int xAlign) {
    float wheel = GetMouseWheelMove();
    if (wheel == 0) return 0; // Pas de mouvement

    // Calcul des limites
    int nbTop = (nbFilms < 5) ? nbFilms : 5;
    int largeurContenu = nbTop * (CARTE_LARGEUR + 15) + 100; 
    int maxScrollX = (largeurContenu + xAlign) - (int)zone.width;
    if (maxScrollX < 0) maxScrollX = 0;

    // Reset si redimensionnement
    if (maxScrollX == 0) *scrollVar = 0;

    // Vérification Souris
    if (CheckCollisionPointRec(GetMousePosition(), zone)) {
        *scrollVar -= wheel * 20.0f; 
        
        // Bornage
        if (*scrollVar < 0) *scrollVar = 0;
        if (*scrollVar > maxScrollX) *scrollVar = maxScrollX;
        
        return 1; // On a consommé l'événement scroll
    }
    return 0;
}

static int dessinerSectionHorizontale(t_catalogue catalogue, Rectangle zoneViewport, int startY, int xAlign, 
                                      char* titre, Color couleur, int* indices, float scrollX) {
    int filmClique = -1;
    int nbFilms = getNbMedia(catalogue);
    int nbTop = (nbFilms < 5) ? nbFilms : 5;
    
    int largeurContenu = nbTop * (CARTE_LARGEUR + 20) + 20; 
    
    Rectangle rectBandeMobile = {
        (float)(xAlign - scrollX),
        zoneViewport.y,
        (float)largeurContenu,
        zoneViewport.height
    };

    BeginScissorMode((int)zoneViewport.x, (int)zoneViewport.y - 40, (int)zoneViewport.width, (int)zoneViewport.height + 50);

        // Cadre et Titre avec la couleur personnalisée
        DrawRectangleLinesEx(rectBandeMobile, 4, couleur);
        DrawText(titre, (int)rectBandeMobile.x + 10, (int)rectBandeMobile.y - 30, 24, couleur);

        for (int i = 0; i < nbTop; i++) {
            int idx = indices[i];
            if (idx != -1) {
                t_media m = getMediaCatalogue(catalogue, idx);
                
                Rectangle rectCarte = {
                    (float)(rectBandeMobile.x + 20 + i * (CARTE_LARGEUR + 20)), 
                    (float)(rectBandeMobile.y + 25), 
                    (float)CARTE_LARGEUR,
                    (float)CARTE_HAUTEUR
                };

                if (mesTextures != NULL) {
                    if (GetMouseY() > startY) {
                       if (dessinerCarteMedia(rectCarte, m, mesTextures[idx])) filmClique = idx;
                    } else {
                       dessinerCarteMedia(rectCarte, m, mesTextures[idx]);
                    }
                }
            }
        }
    
    EndScissorMode(); 
    return filmClique;
}


// --- HELPER : Dessin du Top 5 ---
static int dessinerSectionTop5(t_catalogue catalogue, Rectangle zoneViewport, int startY, int xAlign) {
    int filmClique = -1;
    int nbFilms = getNbMedia(catalogue);
    int nbTop = (nbFilms < 5) ? nbFilms : 5;
    
    // Largeur du contenu (+100 pour être large sur la fin)
    int largeurContenu = nbTop * (CARTE_LARGEUR + 20) + 20; 
    
    // Création de la bande mobile
    Rectangle rectBandeMobile = {
        (float)(xAlign - scrollX_Top5),
        zoneViewport.y,
        (float)largeurContenu,
        zoneViewport.height
    };

    // SCISSOR
    BeginScissorMode((int)zoneViewport.x, (int)zoneViewport.y - 40, (int)zoneViewport.width, (int)zoneViewport.height + 50);

        // Cadre et Titre
        DrawRectangleLinesEx(rectBandeMobile, 4, GREEN);
        DrawText("Top 5 des meilleurs films", (int)rectBandeMobile.x + 10, (int)rectBandeMobile.y - 30, 24, GREEN);

        // Films
        for (int i = 0; i < nbTop; i++) {
            int idx = indicesTop5[i];
            if (idx != -1) {
                t_media m = getMediaCatalogue(catalogue, idx);
                
                Rectangle rectCarte = {
                    (float)(rectBandeMobile.x + 20 + i * (CARTE_LARGEUR + 20)), 
                    (float)(rectBandeMobile.y + 25), 
                    (float)CARTE_LARGEUR,
                    (float)CARTE_HAUTEUR
                };

                if (mesTextures != NULL) {
                    if (GetMouseY() > startY) {
                       if (dessinerCarteMedia(rectCarte, m, mesTextures[idx])) filmClique = idx;
                    } else {
                       dessinerCarteMedia(rectCarte, m, mesTextures[idx]);
                    }
                }
            }
        }
    
    EndScissorMode(); 

    return filmClique;
}

// Remplit un tableau donné avec 5 indices au hasard
static void remplirSelectionAleatoire(int* tableau, int nbTotal) {
    int count = 0;
    int maxFilms = (nbTotal < 5) ? nbTotal : 5;
    
    // On réinitialise le tableau
    for(int i=0; i<5; i++) tableau[i] = -1;

    while (count < maxFilms) {
        int r = GetRandomValue(0, nbTotal - 1);
        int dejaPris = 0;
        for (int i = 0; i < count; i++) {
            if (tableau[i] == r) dejaPris = 1;
        }
        if (!dejaPris) {
            tableau[count] = r;
            count++;
        }
    }
}

// Fonction globale d'initialisation
static void initSections(int nbTotalFilms) {
    if (estSectionsInit || nbTotalFilms == 0) return;
    
    remplirSelectionAleatoire(indicesTop5, nbTotalFilms);
    remplirSelectionAleatoire(indicesSeries, nbTotalFilms);
    remplirSelectionAleatoire(indicesLast, nbTotalFilms);
    
    estSectionsInit = 1;
}

// --- HELPER : Dessin de la Grille Principale ---
static int dessinerSectionGrille(t_catalogue catalogue, int filtre, char* rech, int startX, int currentY, int cols, int espace, int startYHeader) {
    int filmClique = -1;
    int nbFilms = getNbMedia(catalogue);
    int compteurAffiches = 0;

    for (int i = 0; i < nbFilms; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        
        if (mediaCorrespondCategorie(m, filtre) && mediaCorrespondRecherche(m, rech)) {
            
            int colonne = compteurAffiches % cols;
            int ligne = compteurAffiches / cols;

            Rectangle rectCarte;
            rectCarte.x = (float)(startX + colonne * espace);
            rectCarte.y = (float)(currentY + ligne * (CARTE_HAUTEUR + CARTE_PADDING)); // currentY inclut déjà le -scrollY
            rectCarte.width = (float)CARTE_LARGEUR;
            rectCarte.height = (float)CARTE_HAUTEUR;

            if (mesTextures != NULL) {
                // Protection clic header
                if (GetMouseY() > startYHeader) {
                    if (dessinerCarteMedia(rectCarte, m, mesTextures[i])) filmClique = i;
                } else {
                    dessinerCarteMedia(rectCarte, m, mesTextures[i]);
                }
            }
            compteurAffiches++;
        }
    }
    return filmClique;
}

/* FONCTIONS UI =========================================================== */

void initInterface(int largeur, int hauteur, char* titre) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(largeur, hauteur, titre);
    
    // Chargement de l'icône de fenêtre (si dispo)
    if (FileExists("assets/logo.png")) {
        Image icone = LoadImage("assets/logo.png");
        SetWindowIcon(icone); 
        UnloadImage(icone);
    }

    SetTargetFPS(60);
}

void fermerInterface(void) {
    CloseWindow();
}

void chargerTexturesCatalogue(t_catalogue catalogue) {
    nbTexturesChargees = getNbMedia(catalogue);
    if (nbTexturesChargees == 0) return;

    mesTextures = (Texture2D*)malloc(sizeof(Texture2D) * nbTexturesChargees);

    for (int i = 0; i < nbTexturesChargees; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        char cheminImage[150];
        
        sprintf(cheminImage, CHEMIN_IMAGES, getCode(m));

        if (FileExists(cheminImage)) {
            mesTextures[i] = LoadTexture(cheminImage);
            SetTextureFilter(mesTextures[i], TEXTURE_FILTER_BILINEAR);
        } else {
            // Image vide (Magenta) si introuvable
            Image imgVide = GenImageColor(CARTE_LARGEUR, CARTE_HAUTEUR, MAGENTA);
            mesTextures[i] = LoadTextureFromImage(imgVide);
            UnloadImage(imgVide);
        }
    }
}

void libererTexturesCatalogue(void) {
    if (mesTextures != NULL) {
        for (int i = 0; i < nbTexturesChargees; i++) {
            UnloadTexture(mesTextures[i]);
        }
        free(mesTextures);
        mesTextures = NULL;
        nbTexturesChargees = 0;
    }
}


/* FONCTIONS AFFICHAGE ===================================================== */

// Affiche l'En-tête : Fond général, Logo et Titre
void dessinerEnTete(void) {
    Rectangle rectLogo = { 25, 25, 50, 50 };
    DrawRectangleLinesEx(rectLogo, 3, THEME_LOGO_CADRE);
    DrawText("nF", 45, 50, 20, THEME_TITRE);

    // 3. Titre (Juste à droite du logo)
    DrawText("NounaFlix", 100, 40, 40, THEME_TITRE);
}

int dessinerBarreCategories(void) {
    // 1. CONFIGURATION (On garde tes dimensions)
    int hauteur = 50;
    int largeur = 100;
    int gap = 15;           
    int y = 25; 
    int finZoneTitre = 350; 
    
    // On calcule la position pour 5 boutons
    int largeurBarre = (5 * largeur) + (4 * gap);
    int espaceDispo = GetScreenWidth() - finZoneTitre;
    int x = finZoneTitre + (espaceDispo - largeurBarre) / 2;
    if (x < finZoneTitre) x = finZoneTitre;

    int pas = largeur + gap; 
    int choix = -1;

    // 2. RÉORGANISATION DES BOUTONS
    // Bouton 0 : Ajouter (Position 1)
    if (dessinerCarreMenu((Rectangle){(float)x, (float)y, (float)largeur, (float)hauteur}, "Ajouter", COL_BTN_AJOUTER)) choix = 0;
    
    // Bouton 2 : Film (Position 2 - Prend la place de l'ancien Search)
    if (dessinerCarreMenu((Rectangle){(float)(x + pas), (float)y, (float)largeur, (float)hauteur}, "Film", COL_BTN_FILM)) choix = 2;
    
    // Bouton 3 : Serie (Position 3 - Prend la place de l'ancien Film)
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 2), (float)y, (float)largeur, (float)hauteur}, "Serie", COL_BTN_SERIE)) choix = 3;
    
    // Bouton 4 : Autre (Position 4)
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 3), (float)y, (float)largeur, (float)hauteur}, "Autre", COL_BTN_AUTRE)) choix = 4;
    
    // Nouveau Bouton (Position 5) - Exemple "Favoris" ou "Vu"
    // Tu peux définir une nouvelle couleur comme #define COL_BTN_NOUVEAU ORANGE
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 4), (float)y, (float)largeur, (float)hauteur}, "Favoris", GOLD)) choix = 5;
    
    if (choix == -1) SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
    return choix;
}

int dessinerCarteMedia(Rectangle rect, t_media m, Texture2D miniature) {
    int estClique = 0;
    Vector2 souris = GetMousePosition();
    
    Color couleurBordure = CARTE_BORDURE_REPOS;

    // Detection collision
    if (CheckCollisionPointRec(souris, rect)) {
        couleurBordure = CARTE_BORDURE_HOVER; // Changement de couleur au survol
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) estClique = 1;
    }

    // 1. Fond de carte
    DrawRectangleRec(rect, CARTE_FOND);
    
    // 2. Image (Avec espace en bas pour le texte)
    Rectangle rectImage = { rect.x, rect.y, rect.width, rect.height - 50 };
    DrawRectangleRec(rectImage, BLACK); // Fond noir sous l'image
    redimensionTextureMedia(miniature, rectImage);
        
    // 3. Textes
    char titreCoupe[30];
    strncpy(titreCoupe, getTitre(m), 13);
    titreCoupe[13] = '\0'; 
    if(strlen(getTitre(m)) > 13) strcat(titreCoupe, "...");

    DrawText(titreCoupe, (int)rect.x + 8, (int)rect.y + (int)rect.height - 40, 20, CARTE_TEXTE_TITRE);
    DrawText(TextFormat("%d", getAnnee(m)), (int)rect.x + 8, (int)rect.y + (int)rect.height - 20, 10, CARTE_TEXTE_ANNEE);

    // 4. Bordure (Dessinée en dernier pour être au dessus)
    DrawRectangleLinesEx(rect, CARTE_EPAISSEUR_BORD, couleurBordure);

    return estClique;
}

// Affiche le champ de saisie
void dessinerBarreRecherche(char* bufferTexte, int estActif) {
    int largeur = 400;
    int x = (GetScreenWidth() - largeur) / 2;
    int y = 120; 

    Rectangle rect = { (float)x, (float)y, (float)largeur, 30 };

    // Dessin du fond et de la bordure
    DrawRectangleRec(rect, RAYWHITE);
    DrawRectangleLinesEx(rect, 2, LIGHTGRAY);

    // LOGIQUE DU PLACEHOLDER
    if (strlen(bufferTexte) > 0) {
        // Si l'utilisateur a écrit quelque chose, on affiche son texte en noir
        DrawText(bufferTexte, x + 10, y + 5, 20, BLACK);
    } else {
        // Si le buffer est vide, on affiche "Search..." en gris
        DrawText("Search...", x + 10, y + 5, 20, GRAY);
    }
}

int dessinerGrilleFiltree(t_catalogue catalogue, int filtreActif, char* recherche) {
    int filmClique = -1;
    int nbFilms = getNbMedia(catalogue);
    initSections(nbFilms); 

    // --- CONFIGURATION ---
    int startY = 200;
    int largeurFenetre = GetScreenWidth();
    int espaceTotal = CARTE_LARGEUR + CARTE_PADDING;
    int colonnesMax = (largeurFenetre - 80) / espaceTotal;
    if (colonnesMax < 1) colonnesMax = 1;
    int startX = (largeurFenetre - (colonnesMax * espaceTotal)) / 2;

    // --- INPUTS (SCROLL VERTICAL UNIQUEMENT) ---
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        scrollY -= wheel * 30.0f; 
    }

    // --- CALCUL DES HAUTEURS ---
    int afficherSections = (filtreActif == -1 && strlen(recherche) == 0 && nbFilms > 0);
    int hSection = CARTE_HAUTEUR + 90; 
    int hauteurToutesSections = afficherSections ? (hSection * 3) : 0;
    
    // Calcul de la grille principale pour le maxScrollY
    int nbAffichesFiltrees = 0;
    for (int i = 0; i < nbFilms; i++) { 
        t_media m = getMediaCatalogue(catalogue, i);
        if (mediaCorrespondCategorie(m, filtreActif) && mediaCorrespondRecherche(m, recherche)) nbAffichesFiltrees++;
    }
    int nbLignesGrille = (nbAffichesFiltrees + colonnesMax - 1) / colonnesMax;
    int hauteurGrille = nbLignesGrille * (CARTE_HAUTEUR + CARTE_PADDING);
    
    int hauteurTotaleContenu = hauteurToutesSections + hauteurGrille + 100;
    int maxScrollY = hauteurTotaleContenu - (GetScreenHeight() - startY);
    if (maxScrollY < 0) maxScrollY = 0;
    if (scrollY < 0) scrollY = 0;
    if (scrollY > maxScrollY) scrollY = maxScrollY;

    // --- DESSIN ---
    BeginScissorMode(0, startY, largeurFenetre, GetScreenHeight() - startY);

        // Position de départ ajustée par le scroll vertical
        int currentY = startY + 20 - (int)scrollY;

        if (afficherSections) {
            // On réutilise ta fonction mais avec scrollX à 0 (fixe)
            // Zone 1
            Rectangle z1 = { (float)startX, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
            int c1 = dessinerSectionHorizontale(catalogue, z1, startY, startX, "Top 5 Films", GREEN, indicesTop5, 0);
            if (c1 != -1) filmClique = c1;
            currentY += hSection;

            // Zone 2
            Rectangle z2 = { (float)startX, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
            int c2 = dessinerSectionHorizontale(catalogue, z2, startY, startX, "Top 5 Series", PURPLE, indicesSeries, 0);
            if (c2 != -1) filmClique = c2;
            currentY += hSection;

            // Zone 3
            Rectangle z3 = { (float)startX, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
            int c3 = dessinerSectionHorizontale(catalogue, z3, startY, startX, "Derniers vus", ORANGE, indicesLast, 0);
            if (c3 != -1) filmClique = c3;
            currentY += hSection;
        }

        // Grille principale
        int cGrille = dessinerSectionGrille(catalogue, filtreActif, recherche, startX, currentY, colonnesMax, espaceTotal, startY);
        if (cGrille != -1) filmClique = cGrille;

    EndScissorMode();

    return filmClique;
}

// --- NOUVELLE FONCTION : PAGE DETAILS ---
int dessinerPageDetails(t_media m, Texture2D affiche) {
    int action = 0; // 0=Rien, 1=Retour, 2=Play

    // 1. Bouton RETOUR (En haut à gauche)
    Rectangle btnRetour = { 20, 90, 100, 40 }; // Sous le header
    if (dessinerCarreMenu(btnRetour, "< Retour", DARKGRAY)) {
        action = 1;
    }

    // 2. Mise en page
    int startY = 140;
    int imgW = 300;
    int imgH = 450;
    
    // Zone Image (A gauche)
    Rectangle rectImage = { 50, (float)startY, (float)imgW, (float)imgH };

    dessinerEnTete();
    
    // Ombre portée pour faire joli
    DrawRectangle(55, startY + 5, imgW, imgH, Fade(BLACK, 0.5f));
    DrawRectangleRec(rectImage, BLACK);
    redimensionTextureMedia(affiche, rectImage);
    DrawRectangleLinesEx(rectImage, 2, DARKGRAY);

    // Zone Infos (A droite)
    int textX = 380; // A droite de l'image
    int textY = startY;

    // Titre (Gros)
    DrawText(getTitre(m), textX, textY, 40, WHITE);
    
    // Ligne décorative sous le titre
    DrawLine(textX, textY + 50, GetScreenWidth() - 50, textY + 50, CARTE_BORDURE_REPOS);

    // Détails (Lignes espacées)
    textY += 70;
    int taillePolice = 20;
    int ecart = 35;
    Color colLabel = LIGHTGRAY;
    Color colValue = WHITE;

    // On utilise TextFormat pour insérer les variables
    DrawText("Annee :", textX, textY, taillePolice, colLabel);
    DrawText(TextFormat("%d", getAnnee(m)), textX + 150, textY, taillePolice, colValue);
    textY += ecart;

    DrawText("Duree :", textX, textY, taillePolice, colLabel);
    DrawText(TextFormat("%d min", getDuree(m)), textX + 150, textY, taillePolice, colValue); // Attention : getDuree à ajouter dans ton modele.h si pas fait !
    textY += ecart;

    DrawText("Genre :", textX, textY, taillePolice, colLabel);
    DrawText(getType(m), textX + 150, textY, taillePolice, colValue); // Idem pour getGenre
    textY += ecart;
    
    DrawText("Code :", textX, textY, taillePolice, colLabel);
    DrawText(getCode(m), textX + 150, textY, taillePolice, colValue);
    textY += ecart;

    DrawText("Auteur :", textX, textY, taillePolice, colLabel);
    DrawText(getAuteur(m), textX + 150, textY, taillePolice, colValue);
    textY += ecart + 20;

    // 3. Gros Bouton LECTURE
    Rectangle btnPlay = { (float)textX, (float)textY + 20, 200, 60 };
    
    // On dessine le bouton manuellement pour qu'il soit bien rouge
    Color colBtn = CARTE_TEXTE_TITRE; // Rouge Netflix
    if (CheckCollisionPointRec(GetMousePosition(), btnPlay)) {
        colBtn = CARTE_BORDURE_REPOS; // Plus vif au survol
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) action = 2;
    }
    
    DrawRectangleRec(btnPlay, colBtn);
    DrawText("LECTURE", (int)btnPlay.x + 30, (int)btnPlay.y + 15, 30, WHITE);

    return action;
}

/* FONCTIONS ANNIMATION =================================================== */
    
void animLogoStart(void) {

    int logoPositionX = GetScreenWidth()/2 - 128;
    int logoPositionY = GetScreenHeight()/2 - 128;

    int framesCounter = 0;
    int lettersCount = 0;

    int topSideRecWidth = 16;
    int leftSideRecHeight = 16;
    int bottomSideRecWidth = 16;
    int rightSideRecHeight = 16;

    int state = 0; 
    float alpha = 1.0f; 

    // Boucle d'animation (Bloquante : on ne sort pas tant que ce n'est pas fini)
    while (!WindowShouldClose()) 
    {       
        if (state == 0) // Clignotement
        {
            framesCounter++;
            if (framesCounter == 80) // Un peu plus rapide
            {
                state = 1;
                framesCounter = 0;
            }
        }
        else if (state == 1) // Haut et Gauche
        {
            topSideRecWidth += 8;
            leftSideRecHeight += 8;
            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2) // Bas et Droite
        {
            bottomSideRecWidth += 8;
            rightSideRecHeight += 8;
            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3) // Lettres
        {
            framesCounter++;
            if (framesCounter/10)       
            {
                lettersCount++;
                framesCounter = 0;
            }
            if (lettersCount >= 10)     
            {
                alpha -= 0.02f;
                if (alpha <= 0.0f) return; // FIN DE L'ANIMATION -> Retour au main
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (state == 0) {
                if ((framesCounter/10)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            }
            else if (state == 1) {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
            }
            else if (state == 2) {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
                DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
            }
            else if (state == 3) {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

                DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));
                DrawText(TextSubtext("NounaFlix", 0, lettersCount), GetScreenWidth()/2 - 50, GetScreenHeight()/2 + 70, 30, Fade(RED, alpha));
            }
        EndDrawing();
    }
}
