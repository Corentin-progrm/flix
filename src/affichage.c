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

/* VARIABLES GLOBALES ===================================================== */
Texture2D* mesTextures = NULL;          // Tableau dynamique des textures chargées
static int nbTexturesChargees = 0;      // Nombre de textures chargées

/* CONSTANTES ============================================================= */
#define CHEMIN_IMAGES "assets/images/%s.jpg"

/* DEFINITION DES FONCTIONS =============================================== */

void initInterface(int largeur, int hauteur, char* titre) {
    InitWindow(largeur, hauteur, titre);            // Taille de la fenêtre
    
    Image icone = LoadImage("assets/logo.png");     // Charge l'icône
    SetWindowIcon(icone); 
    UnloadImage(icone);

    SetTargetFPS(60);                               // Limite à 60 FPS
}

void fermerInterface(void) {
    CloseWindow();
}

void chargerTexturesCatalogue(t_catalogue catalogue) {
    nbTexturesChargees = getNbMedia(catalogue);         // Nombre de médias dans le catalogue
    if (nbTexturesChargees == 0) return;                // Si vide, rien à faire

    // Allocation dynamique du tableau
    mesTextures = (Texture2D*)malloc(sizeof(Texture2D) * nbTexturesChargees);

    // Chargement des images
    for (int i = 0; i < nbTexturesChargees; i++) {
        t_media m = getMediaCatalogue(catalogue, i);        // Récupère le média
        char cheminImage[100];
        
        sprintf(cheminImage, CHEMIN_IMAGES, getCode(m));    // Construit le chemin de l'image

        if (FileExists(cheminImage)) {
            mesTextures[i] = LoadTexture(cheminImage);      // Charge la texture
            SetTextureFilter(mesTextures[i], TEXTURE_FILTER_BILINEAR);      // Filtrage bilinéaire
        } else {
            // Image par défaut (Magenta)
            Image imgVide = GenImageColor(CARTE_LARGEUR, CARTE_LARGEUR, MAGENTA);
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

void redimensionTextureMedia(Texture2D texture, Rectangle destRect) {
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

void dessinerFondMenu(void) {
    DrawText("NounaFlix", 20, 20, 40, RED);     // Titre

    void dessinerLogo(void);
    //Ajouter le bloque recherche
    //Ajouter le bloque serie
    //Ajouter le bloque film
}

int dessinerCarteMedia(Rectangle rect, t_media m, Texture2D miniature) {
    
    int estClique = 0;
    Vector2 souris = GetMousePosition();
    
    // Couleurs par défaut
    Color couleurBordure = DARKGREEN;
    Color couleurFond = GRAY;
    Color couleurTexte = DARKGREEN;
    int epaisseurBordure = 3;

    // Collision avec la souris
    if (CheckCollisionPointRec(souris, rect)) {
        couleurBordure = BLACK;         // La bordure devient NOIRE
        
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            estClique = 1;
        }
    }

    DrawRectangleRec(rect, couleurFond);    // Dessine le fond de la carte
    
    // Calcul de la zone image
    Rectangle rectImage = { rect.x, rect.y, rect.width, rect.height - 50 };
    
    // Fond sous l'image (au cas où l'image est transparente ou charge mal)
    DrawRectangleRec(rectImage, BLACK); 
    
    // Redimensionnement et dessin de l'image
    redimensionTextureMedia(miniature, rectImage); 
        
    // Préparation du titre (Couper si trop long)
    char titreCoupe[30];
    strncpy(titreCoupe, getTitre(m), 13);
    titreCoupe[13] = '\0'; 
    if(strlen(getTitre(m)) > 13) strcat(titreCoupe, "...");

    // Dessin du texte
    DrawText(titreCoupe, rect.x + 8, rect.y + rect.height - 40, 20, couleurTexte);
    
    // Dessin de l'année
    DrawText(TextFormat("%d", getAnnee(m)), rect.x + 8, rect.y + rect.height - 20, 10, DARKGRAY);

    // Dessin de la bordure
    DrawRectangleLinesEx(rect, epaisseurBordure, couleurBordure);

    return estClique;
}

void animLogoStart(void)
{
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

void dessinerLogo(void) {    
    


}

void dessinerCarrerMenue(void) {


    
}