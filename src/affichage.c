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
Texture2D* mesTextures = NULL;          
static int nbTexturesChargees = 0;
static float scrollY = 0.0f;    

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
    
// 1. CONFIGURATION
    int hauteur = 50;
    int largeur = 100;
    int gap = 15;           
    int y = 25; 

    // 2. CALCUL DE LA POSITION X (Alignement DROITE)
    // On calcule la place totale que prennent les 5 boutons et les 4 espaces
    int largeurTotaleMenu = (5 * largeur) + (4 * gap);
    
    // Formule : LargeurEcran - TailleMenu - MargeSouhaitée(25)
    int x = GetScreenWidth() - largeurTotaleMenu - 25;

    // (Optionnel) Sécurité : Si l'écran est trop petit, on empêche le menu 
    // d'écraser le logo à gauche (on bloque à 150px minimum)
    if (x < 150) x = 150; 

    int pas = largeur + gap; 
    int choix = -1;

    // 3. DESSIN DES BOUTONS
    if (dessinerCarreMenu((Rectangle){(float)x, (float)y, (float)largeur, (float)hauteur}, "Ajouter", COL_BTN_AJOUTER)) choix = 0;
    if (dessinerCarreMenu((Rectangle){(float)(x + pas), (float)y, (float)largeur, (float)hauteur}, "Search", COL_BTN_SEARCH)) choix = 1;
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 2), (float)y, (float)largeur, (float)hauteur}, "Film", COL_BTN_FILM)) choix = 2;
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 3), (float)y, (float)largeur, (float)hauteur}, "Serie", COL_BTN_SERIE)) choix = 3;
    if (dessinerCarreMenu((Rectangle){(float)(x + pas * 4), (float)y, (float)largeur, (float)hauteur}, "Autre", COL_BTN_AUTRE)) choix = 4;
    
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
    // Position : Juste en dessous du menu Catégories
    int largeur = 400;
    int x = (GetScreenWidth() - largeur) / 2;
    int y = 120; // Entre le menu et la grille

    Rectangle rect = { (float)x, (float)y, (float)largeur, 30 };

    // Couleur de la boite
    DrawRectangleRec(rect, RAYWHITE);
    
    // Bordure (Vert si actif, Gris sinon)
    Color colBordure = estActif ? GREEN : LIGHTGRAY;
    DrawRectangleLinesEx(rect, 2, colBordure);

    // Texte à l'intérieur
    DrawText(bufferTexte, x + 10, y + 5, 20, BLACK);

}

/* * Affiche la grille avec défilement vertical.
 * Retourne l'index du film cliqué, ou -1 si rien n'est cliqué.
 */
int dessinerGrilleFiltree(t_catalogue catalogue, int filtreActif, char* recherche) {
    
    int filmClique = -1; // Par défaut, aucun clic
    int nbFilms = getNbMedia(catalogue);
    
// Dans affichage.c -> dessinerGrilleFiltree

    // --- CONFIGURATION DE LA GRILLE DYNAMIQUE ---
    int largeurFenetre = GetScreenWidth();
    int espaceTotalCarte = CARTE_LARGEUR + CARTE_PADDING;
    int margeMinimale = 40; // On veut au moins 40px de chaque côté

    // 1. Calcul du nombre de colonnes possibles
    int colonnesMax = (largeurFenetre - (margeMinimale * 2)) / espaceTotalCarte;
    if (colonnesMax < 1) colonnesMax = 1;

    // 2. Calcul du startX pour CENTRER le bloc de cartes
    // (Largeur fenêtre - Largeur totale des cartes) / 2
    int largeurContenu = colonnesMax * espaceTotalCarte;
    int startX = (largeurFenetre - largeurContenu) / 2;
    
    // Petit ajustement visuel pour centrer par rapport au visuel de la carte
    startX += CARTE_PADDING / 2; 

    int startY = 180;

    // --- GESTION DU SCROLL ---
    
    // 1. Détection de la molette
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        scrollY -= wheel * 20.0f; // Vitesse de scroll
    }

    // 2. Pré-calcul : Compter les films à afficher pour connaitre la hauteur totale
    int nbAffichesFiltrees = 0;
    for (int i = 0; i < nbFilms; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        if (mediaCorrespondCategorie(m, filtreActif) && mediaCorrespondRecherche(m, recherche)) {
            nbAffichesFiltrees++;
        }
    }

    // 3. Calcul des limites du scroll (Clamping)
    int nbLignesNecessaires = (nbAffichesFiltrees + colonnesMax - 1) / colonnesMax;
    int hauteurTotaleContenu = nbLignesNecessaires * (CARTE_HAUTEUR + CARTE_PADDING);
    int hauteurVisible = GetScreenHeight() - startY; 
    
    int maxScroll = hauteurTotaleContenu - hauteurVisible + 20; // Marge du bas
    if (maxScroll < 0) maxScroll = 0; // Pas de scroll nécessaire si peu de films

    // On empêche de scroller trop haut ou trop bas
    if (scrollY < 0) scrollY = 0;
    if (scrollY > maxScroll) scrollY = maxScroll;


    // --- DESSIN (SCISSOR MODE) ---
    
    BeginScissorMode(0, startY, largeurFenetre, hauteurVisible);

    int compteurAffiches = 0;

    for (int i = 0; i < nbFilms; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        
        // On vérifie les filtres
        int matchCategorie = mediaCorrespondCategorie(m, filtreActif);
        int matchRecherche = mediaCorrespondRecherche(m, recherche);

        if (matchCategorie && matchRecherche) {
            
            // Calcul de la position dans la grille
            int colonne = compteurAffiches % colonnesMax;
            int ligne = compteurAffiches / colonnesMax;

            Rectangle rectCarte;
            rectCarte.x = (float)(startX + colonne * espaceTotalCarte);
            // C'est ici qu'on applique le décalage du scroll (- scrollY)
            rectCarte.y = (float)(startY + ligne * (CARTE_HAUTEUR + CARTE_PADDING) - scrollY);
            rectCarte.width = (float)CARTE_LARGEUR;
            rectCarte.height = (float)CARTE_HAUTEUR;

            if (mesTextures != NULL) {
                // IMPORTANT : On vérifie si la souris est physiquement dans la zone de la liste
                // pour éviter de cliquer sur un film caché sous le menu du haut
                if (GetMouseY() > startY) {
                    // Si on clique sur la carte, on sauvegarde l'ID
                    if (dessinerCarteMedia(rectCarte, m, mesTextures[i])) {
                        filmClique = i;
                    }
                } else {
                    // Si la carte est sous le menu, on la dessine juste (sans interaction)
                    dessinerCarteMedia(rectCarte, m, mesTextures[i]);
                }
            }
            compteurAffiches++;
        }
    }
    
    EndScissorMode(); // Fin de la zone de découpe


    // --- BARRE DE DÉFILEMENT (VISUEL) ---
    if (maxScroll > 0) {
        int barreLargeur = 8;
        int barreX = largeurFenetre - 12;
        
        // Taille de l'ascenseur proportionnelle au contenu
        float ratio = (float)hauteurVisible / (hauteurTotaleContenu + hauteurVisible);
        int barreHauteur = (int)(hauteurVisible * ratio);
        if (barreHauteur < 30) barreHauteur = 30; // Taille minimale

        // Position de l'ascenseur
        float barreY = startY + (scrollY / maxScroll) * (hauteurVisible - barreHauteur);
        
        // Fond de la barre
        DrawRectangle(barreX, startY, barreLargeur, hauteurVisible, Fade(LIGHTGRAY, 0.1f));
        // L'ascenseur
        DrawRectangle(barreX, (int)barreY, barreLargeur, barreHauteur, Fade(GRAY, 0.5f));
    }

    return filmClique; // On renvoie l'ID au Main pour qu'il change de page
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