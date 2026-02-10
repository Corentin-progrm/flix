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
const Color COLOR_ACCENT            = { 230, 41, 55, 255 };     // RED Raylib

// En-tête et Recherche
const Color COLOR_HEADER_LINE       = { 0, 0, 0, 255 };         // BLACK Logo cadre
const Color COLOR_SEARCH_BG         = { 116, 128, 139, 255 };      // Fond rectangle Recherche
const Color COLOR_SEARCH_BORDER     = { 74, 82, 89, 255 };       // Bordure rectangle Recherche
const Color COLOR_SEARCH_TEXT       = { 252, 245, 238, 255 };         // Texte Recherche...

// Boutons Catégories (Mode Tuile)
const Color COLOR_BTN_TXT           = { 252, 245, 238, 255 };   // Texte boutons
const Color COLOR_CAT_AJOUTER       = { 255, 230, 230, 255 };     // Cadre Menu Ajouter
const Color COLOR_CAT_FILM          = { 255, 196, 196, 255 };     // Cadre Menu Film
const Color COLOR_CAT_SERIE         = { 238, 105, 131, 255 };     // Cadre Menu Serie
const Color COLOR_CAT_AUTRE         = { 188, 21, 54, 255 };     // Cadre Menu Autre
const Color COLOR_CAT_FAVORI        = { 133, 14, 53, 255 };     // Cadre Menu Favoris

// Cartes Média
const Color COLOR_CARD_BG           = { 116, 128, 139, 255 };      // Fond carte média
const Color COLOR_CARD_TITLE        = { 252, 245, 238, 255 };   // Titre carte média
const Color COLOR_CARD_YEAR         = { 200, 200, 200, 200 };   // Année carte média
const Color COLOR_CARD_BORDER_OFF   = { 74, 82, 89, 255 };       // Bordure carte média
const Color COLOR_CARD_BORDER_ON    = { 0, 0, 0, 255 };         // Bordure carte média hover

// Page Détails
const Color COLOR_DET_BG            = { 51, 56, 61, 255 };      // Background application
const Color COLOR_DET_TITLE         = { 252, 245, 238, 255 };   // Titre principal
const Color COLOR_DET_LABEL         = { 252, 245, 238, 255 };   // Labels des champs
const Color COLOR_DET_LINE          = { 74, 82, 89, 255 };       // Ligne de séparation
const Color COLOR_DET_VALUE         = { 252, 245, 238, 255 };   // Valeurs des champs
const Color COLOR_DET_PLAY_BTN_OFF  = { 74, 82, 89, 255 };       // Cadre Bouton Play
const Color COLOR_DET_PLAY_BTN_ON   = { 0, 0, 0, 255 };         // Cadre Bouton Play hover
const Color COLOR_CAT_RETOUR        = { 74, 82, 89, 255 };       // Cadre Bouton Retour

// Sections Horizontales
const Color COLOR_HORZ_FILM       = { 188, 204, 220, 255 };       // Section Top 5 Films
const Color COLOR_HORZ_SERIE      = { 217, 234, 253, 255 };       // Section Top 5 Series
const Color COLOR_HORZ_LAST       = { 248, 250, 252, 255 };       // Section Derniers Vus




#define CHEMIN_IMAGES "assets/images/%s.jpg"

/* ============================================================================
   VARIABLES GLOBALES
   ============================================================================ */

Texture2D* mesTextures = NULL;          
static int nbTexturesChargees = 0;
static float scrollY = 0.0f;    
static float scrollYDetails = 0.0f;

static int indicesTop5[5] = {-1, -1, -1, -1, -1};
static int indicesSeries[5] = {-1, -1, -1, -1, -1};
static int indicesLast[5] = {-1, -1, -1, -1, -1};
static int estSectionsInit = 0;

static Font policeApp;

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

    DessinerTextePerso(texte, posX, posY, taillePolice, COLOR_BTN_TXT);

    return estClique;
}


/**
 * @fonction initSections
 * @brief Initialise les sélections (aléatoires pour Films/Séries, historique pour le reste).
 * @param catalogue Le catalogue complet pour rechercher les médias par type ou code.
 */
static void initSections(t_catalogue catalogue) {
    if (estSectionsInit) return; //
    
    int nbTotal = getNbMedia(catalogue); //
    
    // Tableaux temporaires pour le tirage aléatoire
    int idxFilms[100], nbF = 0;
    int idxSeries[100], nbS = 0;

    // 1. Initialisation des tableaux d'indices à -1 (vide)
    for(int i = 0; i < 5; i++) {
        indicesTop5[i] = -1;  //
        indicesSeries[i] = -1; //
        indicesLast[i] = -1;   //
    }

    // 2. Tri du catalogue pour préparer le tirage aléatoire des Films et Séries
    for (int i = 0; i < nbTotal; i++) {
        t_media m = getMediaCatalogue(catalogue, i); //
        char* type = getType(m); //

        if (strcmp(type, "Film") == 0 && nbF < 100) {
            idxFilms[nbF++] = i;
        }
        else if (strcmp(type, "Serie") == 0 && nbS < 100) {
            idxSeries[nbS++] = i;
        }
    }

    // 3. Tirage aléatoire UNIQUE pour les FILMS (Fisher-Yates)
    for (int i = 0; i < 5 && i < nbF; i++) {
        int r = GetRandomValue(i, nbF - 1); //
        int temp = idxFilms[i];
        idxFilms[i] = idxFilms[r];
        idxFilms[r] = temp;
        indicesTop5[i] = idxFilms[i]; 
    }

    // 4. Tirage aléatoire UNIQUE pour les SERIES
    for (int i = 0; i < 5 && i < nbS; i++) {
        int r = GetRandomValue(i, nbS - 1); //
        int temp = idxSeries[i];
        idxSeries[i] = idxSeries[r];
        idxSeries[r] = temp;
        indicesSeries[i] = idxSeries[i];
    }

    // 5. CHARGEMENT DE L'HISTORIQUE PERSISTANT (Derniers vus)
    FILE* f = fopen("assets/historique.txt", "r");
    if (f) {
        char codeLu[50];
        int count = 0;
        // On lit les codes ligne par ligne dans le fichier
        while (fgets(codeLu, sizeof(codeLu), f) && count < 5) {
            // Nettoyage du caractère de saut de ligne \n
            codeLu[strcspn(codeLu, "\n")] = 0;
            
            // Recherche de l'index correspondant au code dans le catalogue
            for (int j = 0; j < nbTotal; j++) {
                t_media m = getMediaCatalogue(catalogue, j);
                if (strcmp(getCode(m), codeLu) == 0) {
                    indicesLast[count] = j;
                    count++;
                    break; 
                }
            }
        }
        fclose(f);
    }
    
    estSectionsInit = 1; //
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
    DessinerTextePerso(titre, (int)rectBandeMobile.x + 10, (int)rectBandeMobile.y - 30, 24, couleur);

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
    
    policeApp = LoadFontEx("assets/VarelaRound-Regular.ttf", 128, NULL, 0);
    SetTextureFilter(policeApp.texture, TEXTURE_FILTER_POINT);

    if (FileExists("assets/logo.png")) {
        Image icone = LoadImage("assets/logo.png");
        SetWindowIcon(icone); 
        UnloadImage(icone);
    }

    SetTargetFPS(60);
}

void fermerInterface(void) {
    UnloadFont(policeApp);
    CloseWindow();
}

void DessinerTextePerso(const char* texte, int x, int y, int taille, Color couleur) {
    Vector2 pos = { (float)x, (float)y };
    DrawTextEx(policeApp, texte, pos, (float)taille+3, 1, couleur);
}

void chargerTexturesCatalogue(t_catalogue catalogue) {
    nbTexturesChargees = getNbMedia(catalogue);
    if (nbTexturesChargees == 0) return;

    mesTextures = (Texture2D*)malloc(sizeof(Texture2D) * nbTexturesChargees);

    for (int i = 0; i < nbTexturesChargees; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        char cheminImage[150];
        
        char* codeImage = getCode(m);

        if (strcmp(getType(m), "Episode") == 0) {
            codeImage = getParent(m); 
        }

        // 2. Construire le chemin final
        sprintf(cheminImage, CHEMIN_IMAGES, codeImage);

        // 3. Vérifier et charger
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
    DessinerTextePerso("nF", 45, 50, 20, COLOR_ACCENT);
    DessinerTextePerso("NounaFlix", 100, 40, 40, COLOR_ACCENT);
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
    
    // --- DANS affichage.c (dessinerCarteMedia) ---

    char titreAffichage[30];
    char* texteSource;

    // 1. On détermine quelle chaîne on veut afficher
    if (strcmp(getType(m), "Episode") == 0) {
        texteSource = getEpisode(m); // Utilise "S01E01" par exemple
    } else {
        texteSource = getTitre(m);   // Utilise "Inception"
    }

    // 2. On vérifie la longueur réelle avant de couper
    if (texteSource != NULL && strlen(texteSource) > 13) {
        strncpy(titreAffichage, texteSource, 13);
        titreAffichage[13] = '\0'; 
        strcat(titreAffichage, "...");
    } else if (texteSource != NULL) {
        strcpy(titreAffichage, texteSource); // Pas besoin de couper
    } else {
        strcpy(titreAffichage, "Inconnu");
    }

    // 3. On affiche le résultat
    DessinerTextePerso(titreAffichage, (int)rect.x + 8, (int)rect.y + (int)rect.height - 40, 20, COLOR_CARD_TITLE);
    DessinerTextePerso(TextFormat("%d", getAnnee(m)), (int)rect.x + 8, (int)rect.y + (int)rect.height - 20, 10, COLOR_CARD_YEAR);

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
        DessinerTextePerso(bufferTexte, x + 10, y + 5, 20, COLOR_SEARCH_TEXT);
    } else {
        DessinerTextePerso("Rechercher...", x + 10, y + 5, 20, COLOR_SEARCH_TEXT);
    }
}

int dessinerGrilleFiltree(t_catalogue catalogue, int filtreActif, char* recherche) {
    int filmClique = -1;
    int nbFilms = getNbMedia(catalogue);
    initSections(catalogue); 

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
        int largeurTop5 = (5 * CARTE_LARGEUR) + (4 * 20); 
        int startXSections = (largeurFenetre - largeurTop5) / 2;

        Rectangle z3 = { (float)startXSections, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
        int c3 = dessinerSectionHorizontale(catalogue, z3, startY, startXSections, "Derniers vus", COLOR_HORZ_LAST, indicesLast, 0);
        if (c3 != -1) filmClique = c3;
        currentY += hSection;

        Rectangle z1 = { (float)startXSections, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
        int c1 = dessinerSectionHorizontale(catalogue, z1, startY, startXSections, "Top 5 Films", COLOR_HORZ_FILM, indicesTop5, 0);
        if (c1 != -1) filmClique = c1;
        currentY += hSection;

        Rectangle z2 = { (float)startXSections, (float)currentY, (float)largeurFenetre, (float)CARTE_HAUTEUR + 50 };
        int c2 = dessinerSectionHorizontale(catalogue, z2, startY, startXSections, "Top 5 Series", COLOR_HORZ_SERIE, indicesSeries, 0);
        if (c2 != -1) filmClique = c2;
        currentY += hSection;
    }

    int cGrille = dessinerSectionGrille(catalogue, filtreActif, recherche, startX, currentY, 
                                        colonnesMax, espaceTotal, startY);
    if (cGrille != -1) filmClique = cGrille;

    EndScissorMode();

    return filmClique;
}

int dessinerPageDetails(t_media m, Texture2D affiche, t_catalogue catalogue, Texture2D* toutesTextures, int* indexActuel) {
    int action = 0;

    // --- 1. CALCULS PRÉALABLES POUR LE SCROLL ---
    t_media sugg[5];
    int nbSugg = (strcmp(getType(m), "Film") == 0) ? recupererSuggestions(catalogue, m, sugg) : 0;
    
    t_media epis[100];
    int nbEpi = (strcmp(getType(m), "Serie") == 0) ? recupererEpisodesSerie(catalogue, getCode(m), epis) : 0;

    // Calcul de la hauteur pour les limites du scroll
    int hauteurInfos = 450 + 50; 
    int hauteurDynamique = (nbSugg > 0) ? 350 : (nbEpi > 0 ? 40 + ((nbEpi + 4) / 5) * 280 : 0);
    int limiteBasse = (GetScreenHeight() - 140) - (hauteurInfos + hauteurDynamique);

    scrollYDetails += GetMouseWheelMove() * 45;
    if (scrollYDetails > 0) scrollYDetails = 0;
    if (limiteBasse < 0 && scrollYDetails < limiteBasse) scrollYDetails = (float)limiteBasse;
    if (limiteBasse >= 0) scrollYDetails = 0;

    int offsetScroll = (int)scrollYDetails;

    // --- 2. DESSIN DU CONTENU SCROLLABLE ---
    BeginScissorMode(0, 140, GetScreenWidth(), GetScreenHeight() - 140);

        int startY = 140 + offsetScroll; 
        Rectangle rectImage = { 50, (float)startY, 300, 450 };

        // Affiche et Détails (Ton style actuel)
        DrawRectangle(55, startY + 5, 300, 450, Fade(BLACK, 0.4f));
        redimensionTextureMedia(affiche, rectImage);
        DrawRectangleLinesEx(rectImage, 2, COLOR_DET_LABEL);

        int textX = 380, textY = startY;
        DessinerTextePerso(getTitre(m), textX, textY, 40, COLOR_DET_TITLE);
        DrawLine(textX, textY + 50, GetScreenWidth() - 50, textY + 50, COLOR_DET_LINE);

        // Affichage des champs
        int ecart = 35;
        DessinerTextePerso(TextFormat("Annee : %d", getAnnee(m)), textX, textY + 70, 20, COLOR_DET_VALUE);
        DessinerTextePerso(TextFormat("Duree : %d min", getDuree(m)), textX, textY + 70 + ecart, 20, COLOR_DET_VALUE);
        DessinerTextePerso(TextFormat("Auteur : %s", getAuteur(m)), textX, textY + 70 + ecart*2, 20, COLOR_DET_VALUE);

        // Bouton Lecture
        if (strcmp(getType(m), "Serie") != 0) {
            Rectangle btnPlay = { (float)textX, (float)textY + 180, 200, 60 };
            if (CheckCollisionPointRec(GetMousePosition(), btnPlay)) {
                DrawRectangleRec(btnPlay, COLOR_DET_PLAY_BTN_ON);
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) action = 2;
            } else {
                DrawRectangleRec(btnPlay, COLOR_DET_PLAY_BTN_OFF);
            }
            DessinerTextePerso("LECTURE", (int)btnPlay.x + 35, (int)btnPlay.y + 15, 25, WHITE);
        }

        // --- SECTION SUGGESTIONS ---
        int zoneY = startY + 450 + 50;
        DrawLine(50, zoneY - 20, GetScreenWidth() - 50, zoneY - 20, COLOR_DET_LINE);

        if (nbSugg > 0) {
            DessinerTextePerso("DU MEME AUTEUR :", 50, zoneY, 20, COLOR_DET_LABEL);
            for (int i = 0; i < nbSugg; i++) {
                float posX = 50 + (i * 180), posY = (float)zoneY + 40;
                int idxTex = -1;
                // Trouver l'index de la suggestion dans le catalogue global
                for(int j=0; j < getNbMedia(catalogue); j++) {
                    if(getMediaCatalogue(catalogue, j) == sugg[i]) { idxTex = j; break; }
                }
                if (idxTex != -1) {
                    if (dessinerCarteMedia((Rectangle){ posX, posY, 150, 225 }, sugg[i], toutesTextures[idxTex])) {
                        *indexActuel = idxTex; // On change le média sélectionné
                        scrollYDetails = 0;    // Reset le scroll pour le nouveau film
                    }
                }
            }
        } else if (nbEpi > 0) {
            DessinerTextePerso("EPISODES :", 50, zoneY, 20, COLOR_DET_LABEL);
            for (int i = 0; i < nbEpi; i++) {
                float posX = 50 + ((i % 5) * 180);
                float posY = (float)zoneY + 40 + ((i / 5) * 280);
                if (dessinerCarteMedia((Rectangle){ posX, posY, 150, 225 }, epis[i], affiche)) {
                    lancerVideo(epis[i]); // Lance l'épisode
                }
            }
        }

    EndScissorMode();

    // --- 3. INTERFACE FIXE (Header + Retour) ---
    DrawRectangle(0, 0, GetScreenWidth(), 140, COLOR_DET_BG); 
    dessinerEnTete();
    if (dessinerCarreMenu((Rectangle){ 20, 90, 100, 40 }, "< Retour", COLOR_CAT_RETOUR)) {
        action = 1; // Retour à l'accueil
        scrollYDetails = 0; 
    }

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
            DessinerTextePerso(TextSubtext("NounaFlix", 0, lettersCount), GetScreenWidth()/2 - 50, 
                     GetScreenHeight()/2 + 70, 30, Fade(RED, alpha));
        }
        EndDrawing();
    }
}