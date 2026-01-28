/**
 * Module affichage - affichage.c
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module affichage gère toutes les fonctions
 *               d'affichage du programme, y compris les menus
 *              et les détails des mediass.
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "affichage.h"

/* ============================================================================
   CONSTANTES - COULEURS ET STYLE
   ============================================================================ */

// Thème Général et Fond
const Color COLOR_ACCENT          = { 255, 255, 255, 255 };   // Rouge NounaFlix

// En-tête et Recherche
const Color COLOR_HEADER_LINE     = { 255, 255, 255, 255 };
const Color COLOR_SEARCH_BG       = { 255, 255, 255, 255 }; // RAYWHITE
const Color COLOR_SEARCH_BORDER   = { 255, 255, 255, 255 }; // LIGHTGRAY
const Color COLOR_SEARCH_TEXT     = { 255, 255, 255, 255 };

// Boutons Catégories (Mode Tuile)
const Color COLOR_BTN_TXT         = { 255, 255, 255, 255 }; // RAYWHITE
const Color COLOR_CAT_AJOUTER     = { 255, 255, 255, 255 }; // LIGHTGRAY
const Color COLOR_CAT_FILM        = { 255, 255, 255, 255 };     // MAROON
const Color COLOR_CAT_SERIE       = { 255, 255, 255, 255 };   // PURPLE
const Color COLOR_CAT_AUTRE       = { 255, 255, 255, 255 };   // GOLD
const Color COLOR_CAT_FAVORI      = { 255, 255, 255, 255 };   // GOLD
const Color COLOR_CAT_RETOUR      = { 255, 255, 255, 255 };    // DARKGRAY

// Cartes Média
const Color COLOR_CARD_BG         = { 255, 255, 255, 255 };
const Color COLOR_CARD_TITLE      = { 255, 255, 255, 255 };
const Color COLOR_CARD_YEAR       = { 255, 255, 255, 255 };
const Color COLOR_CARD_BORDER_OFF = { 255, 255, 255, 255 };
const Color COLOR_CARD_BORDER_ON  = { 255, 255, 255, 255 };  // Rouge au survol

// Page Détails
const Color COLOR_DET_TITLE       = { 255, 255, 255, 255 };
const Color COLOR_DET_LABEL       = { 255, 255, 255, 255 }; // Gris clair pour "Année :"
const Color COLOR_DET_VALUE       = { 255, 255, 255, 255 }; // Blanc pour les valeurs
const Color COLOR_DET_PLAY_BTN    = { 255, 255, 255, 255 };

// Sections Horizontales
const Color COLOR_HORZ_FILM       = { 255, 255, 255, 255 };
const Color COLOR_HORZ_SERIE      = { 255, 255, 255, 255 };
const Color COLOR_HORZ_LAST       = { 255, 255, 255, 255 };

#define CHEMIN_IMAGES "assets/images/%s.jpg"

/* ============================================================================
   VARIABLES GLOBALES
   ============================================================================ */

Texture2D* mesTextures = NULL;          
static int nbTexturesChargees = 0;
static float scrollY = 0.0f;    

static int indicesTop5[5] = {-1, -1, -1, -1, -1};
static int indicesSeries[5] = {-1, -1, -1, -1, -1};
static int indicesLast[5] = {-1, -1, -1, -1, -1};
static int estSectionsInit = 0;

/* ============================================================================
   FONCTIONS UTILITAIRES PRIVÉES
   ============================================================================ */

// Redimensionne une image pour remplir un rectangle (Mode COVER)
static void redimensionTextureMedia(Texture2D texture, Rectangle destRect) {
    if (texture.id <= 0) return;

    float scaleX = destRect.width / (float)texture.width;
    float scaleY = destRect.height / (float)texture.height;
    float scale = (scaleX > scaleY) ? scaleX : scaleY;

    float texWidthZoom = texture.width * scale;
    float texHeightZoom = texture.height * scale;

    Vector2 pos = { 
        destRect.x + (destRect.width - texWidthZoom) / 2.0f, 
        destRect.y + (destRect.height - texHeightZoom) / 2.0f 
    };

    BeginScissorMode((int)destRect.x, (int)destRect.y, (int)destRect.width, (int)destRect.height);
    DrawTextureEx(texture, pos, 0.0f, scale, WHITE);
    EndScissorMode();
}

// Dessine un bouton carré style "Tuile"
static int dessinerCarreMenu(Rectangle rect, char* texte, Color couleurPrincipale) {
    int estClique = 0;
    Vector2 souris = GetMousePosition();
    
    Color couleurFond = Fade(couleurPrincipale, 0.3f); 
    
    if (CheckCollisionPointRec(souris, rect)) {
        couleurFond = Fade(couleurPrincipale, 0.6f);
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) estClique = 1;
    }

    DrawRectangleRec(rect, couleurFond);               
    DrawRectangleLinesEx(rect, 4, couleurPrincipale);

    int taillePolice = 20;
    int largeurTexte = MeasureText(texte, taillePolice);
    int posX = (int)(rect.x + rect.width - largeurTexte - 10);
    int posY = (int)(rect.y + rect.height - taillePolice - 10);

    DrawText(texte, posX, posY, taillePolice, COLOR_BTN_TXT);

    return estClique;
}

// Remplit un tableau avec 5 indices aléatoires
static void remplirSelectionAleatoire(int* tableau, int nbTotal) {
    int count = 0;
    int maxFilms = (nbTotal < 5) ? nbTotal : 5;
    
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

static void initSections(int nbTotalFilms) {
    if (estSectionsInit || nbTotalFilms == 0) return;
    
    remplirSelectionAleatoire(indicesTop5, nbTotalFilms);
    remplirSelectionAleatoire(indicesSeries, nbTotalFilms);
    remplirSelectionAleatoire(indicesLast, nbTotalFilms);
    
    estSectionsInit = 1;
}

// Dessine une section horizontale de films
static int dessinerSectionHorizontale(t_catalogue catalogue, Rectangle zoneViewport, int startY, 
                                      int xAlign, char* titre, Color couleur, int* indices, float scrollX) {
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

    BeginScissorMode((int)zoneViewport.x, (int)zoneViewport.y - 40, 
                     (int)zoneViewport.width, (int)zoneViewport.height + 50);

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

// Dessine la grille principale de films
static int dessinerSectionGrille(t_catalogue catalogue, int filtre, char* rech, int startX, 
                                 int currentY, int cols, int espace, int startYHeader) {
    int filmClique = -1;
    int nbFilms = getNbMedia(catalogue);
    int compteurAffiches = 0;

    for (int i = 0; i < nbFilms; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        
        if (mediaCorrespondCategorie(m, filtre) && mediaCorrespondRecherche(m, rech)) {
            
            int colonne = compteurAffiches % cols;
            int ligne = compteurAffiches / cols;

            Rectangle rectCarte = {
                (float)(startX + colonne * espace),
                (float)(currentY + ligne * (CARTE_HAUTEUR + CARTE_PADDING)),
                (float)CARTE_LARGEUR,
                (float)CARTE_HAUTEUR
            };

            if (mesTextures != NULL) {
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

/* ============================================================================
   FONCTIONS PUBLIQUES - INTERFACE
   ============================================================================ */

void initInterface(int largeur, int hauteur, char* titre) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(largeur, hauteur, titre);
    
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

/* ============================================================================
   FONCTIONS PUBLIQUES - AFFICHAGE
   ============================================================================ */

void dessinerEnTete(void) {
    Rectangle rectLogo = { 25, 25, 50, 50 };
    DrawRectangleLinesEx(rectLogo, 3, COLOR_HEADER_LINE);
    DrawText("nF", 45, 50, 20, COLOR_ACCENT);
    DrawText("NounaFlix", 100, 40, 40, COLOR_ACCENT);
}

int dessinerBarreCategories(void) {
    int hauteur = 50, largeur = 100, gap = 15, y = 25; 
    int finZoneTitre = 350; 
    
    int largeurBarre = (5 * largeur) + (4 * gap);
    int espaceDispo = GetScreenWidth() - finZoneTitre;
    int x = finZoneTitre + (espaceDispo - largeurBarre) / 2;
    if (x < finZoneTitre) x = finZoneTitre;

    int pas = largeur + gap; 
    int choix = -1;

    // Utilisation des constantes de catégorie
    if (dessinerCarreMenu((Rectangle){(float)x, (float)y, (float)largeur, (float)hauteur}, 
        "Ajouter", COLOR_CAT_AJOUTER)) choix = 0;
    
    if (dessinerCarreMenu((Rectangle){(float)(x + pas), (float)y, (float)largeur, (float)hauteur}, 
        "Film", COLOR_CAT_FILM)) choix = 2;
    
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 2), (float)y, (float)largeur, (float)hauteur}, 
        "Serie", COLOR_CAT_SERIE)) choix = 3;
    
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 3), (float)y, (float)largeur, (float)hauteur}, 
        "Autre", COLOR_CAT_AUTRE)) choix = 4;
    
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 4), (float)y, (float)largeur, (float)hauteur}, 
        "Favoris", COLOR_CAT_FAVORI)) choix = 5;
    
    if (choix == -1) SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
    return choix;
}

int dessinerCarteMedia(Rectangle rect, t_media m, Texture2D miniature) {
    int estClique = 0;
    Vector2 souris = GetMousePosition();
    Color couleurBordure = COLOR_CARD_BORDER_OFF;

    if (CheckCollisionPointRec(souris, rect)) {
        couleurBordure = COLOR_CARD_BORDER_ON;
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) estClique = 1;
    }

    DrawRectangleRec(rect, COLOR_CARD_BG);
    
    Rectangle rectImage = { rect.x, rect.y, rect.width, rect.height - 50 };
    DrawRectangleRec(rectImage, BLACK); // Fond noir pour le mode "Cover"
    redimensionTextureMedia(miniature, rectImage);
        
    char titreCoupe[30];
    strncpy(titreCoupe, getTitre(m), 13);
    titreCoupe[13] = '\0'; 
    if(strlen(getTitre(m)) > 13) strcat(titreCoupe, "...");

    DrawText(titreCoupe, (int)rect.x + 8, (int)rect.y + (int)rect.height - 40, 20, COLOR_CARD_TITLE);
    DrawText(TextFormat("%d", getAnnee(m)), (int)rect.x + 8, (int)rect.y + (int)rect.height - 20, 10, COLOR_CARD_YEAR);

    DrawRectangleLinesEx(rect, 3, couleurBordure); // Épaisseur fixe à 3

    return estClique;
}

void dessinerBarreRecherche(char* bufferTexte) {
    int largeur = 400;
    int x = (GetScreenWidth() - largeur) / 2;
    int y = 120; 
    Rectangle rect = { (float)x, (float)y, (float)largeur, 30 };

    DrawRectangleRec(rect, COLOR_SEARCH_BG);
    DrawRectangleLinesEx(rect, 2, COLOR_SEARCH_BORDER);

    if (strlen(bufferTexte) > 0) {
        DrawText(bufferTexte, x + 10, y + 5, 20, COLOR_SEARCH_TEXT);
    } else {
        DrawText("Rechercher...", x + 10, y + 5, 20, COLOR_SEARCH_TEXT);
    }
}

int dessinerGrilleFiltree(t_catalogue catalogue, int filtreActif, char* recherche) {
    int filmClique = -1;
    int nbFilms = getNbMedia(catalogue);
    initSections(nbFilms); 

    int startY = 200;
    int largeurFenetre = GetScreenWidth();
    int espaceTotal = CARTE_LARGEUR + CARTE_PADDING;
    int colonnesMax = (largeurFenetre - 80) / espaceTotal;
    if (colonnesMax < 1) colonnesMax = 1;
    int startX = (largeurFenetre - (colonnesMax * espaceTotal)) / 2;

    // Scroll vertical
    float wheel = GetMouseWheelMove();
    if (wheel != 0) scrollY -= wheel * 30.0f;

    // Calcul des hauteurs
    int afficherSections = (filtreActif == -1 && strlen(recherche) == 0 && nbFilms > 0);
    int hSection = CARTE_HAUTEUR + 90; 
    int hauteurToutesSections = afficherSections ? (hSection * 3) : 0;
    
    int nbAffichesFiltrees = 0;
    for (int i = 0; i < nbFilms; i++) { 
        t_media m = getMediaCatalogue(catalogue, i);
        if (mediaCorrespondCategorie(m, filtreActif) && mediaCorrespondRecherche(m, recherche)) 
            nbAffichesFiltrees++;
    }
    int nbLignesGrille = (nbAffichesFiltrees + colonnesMax - 1) / colonnesMax;
    int hauteurGrille = nbLignesGrille * (CARTE_HAUTEUR + CARTE_PADDING);
    
    int hauteurTotaleContenu = hauteurToutesSections + hauteurGrille + 100;
    int maxScrollY = hauteurTotaleContenu - (GetScreenHeight() - startY);
    if (maxScrollY < 0) maxScrollY = 0;
    if (scrollY < 0) scrollY = 0;
    if (scrollY > maxScrollY) scrollY = maxScrollY;

    // Dessin
    BeginScissorMode(0, startY, largeurFenetre, GetScreenHeight() - startY);

    int currentY = startY + 20 - (int)scrollY;

    if (afficherSections) {
        Rectangle z1 = { (float)startX, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
        int c1 = dessinerSectionHorizontale(catalogue, z1, startY, startX, "Top 5 Films", COLOR_HORZ_FILM, indicesTop5, 0);
        if (c1 != -1) filmClique = c1;
        currentY += hSection;

        Rectangle z2 = { (float)startX, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
        int c2 = dessinerSectionHorizontale(catalogue, z2, startY, startX, "Top 5 Series", COLOR_HORZ_SERIE, indicesSeries, 0);
        if (c2 != -1) filmClique = c2;
        currentY += hSection;

        Rectangle z3 = { (float)startX, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
        int c3 = dessinerSectionHorizontale(catalogue, z3, startY, startX, "Derniers vus", COLOR_HORZ_LAST, indicesLast, 0);
        if (c3 != -1) filmClique = c3;
        currentY += hSection;
    }

    int cGrille = dessinerSectionGrille(catalogue, filtreActif, recherche, startX, currentY, 
                                        colonnesMax, espaceTotal, startY);
    if (cGrille != -1) filmClique = cGrille;

    EndScissorMode();

    return filmClique;
}

int dessinerPageDetails(t_media m, Texture2D affiche) {
    int action = 0;

    // Bouton Retour avec couleur dédiée
    Rectangle btnRetour = { 20, 90, 100, 40 };
    if (dessinerCarreMenu(btnRetour, "< Retour", COLOR_CAT_RETOUR)) action = 1;

    int startY = 140, imgW = 300, imgH = 450;
    Rectangle rectImage = { 50, (float)startY, (float)imgW, (float)imgH };

    dessinerEnTete();
    
    // Ombre de l'affiche
    DrawRectangle(55, startY + 5, imgW, imgH, Fade(BLACK, 0.4f));
    DrawRectangleRec(rectImage, BLACK);
    redimensionTextureMedia(affiche, rectImage);
    DrawRectangleLinesEx(rectImage, 2, COLOR_DET_LABEL);

    int textX = 380, textY = startY;

    // Titre principal
    DrawText(getTitre(m), textX, textY, 40, COLOR_DET_TITLE);
    DrawLine(textX, textY + 50, GetScreenWidth() - 50, textY + 50, COLOR_ACCENT);

    textY += 70;
    int ecart = 35;

    // Champs de détails utilisant LABEL et VALUE
    DrawText("Annee :", textX, textY, 20, COLOR_DET_LABEL);
    DrawText(TextFormat("%d", getAnnee(m)), textX + 150, textY, 20, COLOR_DET_VALUE);
    textY += ecart;

    DrawText("Duree :", textX, textY, 20, COLOR_DET_LABEL);
    DrawText(TextFormat("%d min", getDuree(m)), textX + 150, textY, 20, COLOR_DET_VALUE);
    textY += ecart;

    DrawText("Genre :", textX, textY, 20, COLOR_DET_LABEL);
    DrawText(getType(m), textX + 150, textY, 20, COLOR_DET_VALUE);
    textY += ecart;
    
    DrawText("Auteur :", textX, textY, 20, COLOR_DET_LABEL);
    DrawText(getAuteur(m), textX + 150, textY, 20, COLOR_DET_VALUE);

    // Bouton de Lecture
    Rectangle btnPlay = { (float)textX, (float)textY + 60, 200, 60 };
    Color colBtn = COLOR_ACCENT;
    if (CheckCollisionPointRec(GetMousePosition(), btnPlay)) {
        colBtn = COLOR_DET_PLAY_BTN; // Assombrir au survol
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) action = 2;
    }
    
    DrawRectangleRec(btnPlay, colBtn);
    DrawText("LECTURE", (int)btnPlay.x + 35, (int)btnPlay.y + 15, 25, WHITE);

    return action;
}

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

    while (!WindowShouldClose()) 
    {       
        if (state == 0) {
            framesCounter++;
            if (framesCounter == 80) {
                state = 1;
                framesCounter = 0;
            }
        }
        else if (state == 1) {
            topSideRecWidth += 8;
            leftSideRecHeight += 8;
            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2) {
            bottomSideRecWidth += 8;
            rightSideRecHeight += 8;
            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3) {
            framesCounter++;
            if (framesCounter/10) {
                lettersCount++;
                framesCounter = 0;
            }
            if (lettersCount >= 10) {
                alpha -= 0.02f;
                if (alpha <= 0.0f) return;
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
            DrawText(TextSubtext("NounaFlix", 0, lettersCount), GetScreenWidth()/2 - 50, 
                     GetScreenHeight()/2 + 70, 30, Fade(RED, alpha));
        }
        EndDrawing();
    }
}