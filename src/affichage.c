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

/*

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



// --- Variables internes pour stocker les images ---
Texture2D* mesTextures = NULL; // Tableau dynamique de textures
static int nbTexturesChargees = 0;

void initInterface(int largeur, int hauteur, char* titre) {
    InitWindow(largeur, hauteur, titre);
    SetTargetFPS(60);
}

void fermerInterface(void) {
    CloseWindow();
}

// Fonction utilitaire interne pour redimensionner l'image dans le cadre
// (Ne t'inquiète pas trop des maths ici, ça centre l'image)
void DrawTextureScaled(Texture2D texture, Rectangle destRect) {
    if (texture.id <= 0) return; // Sécurité si la texture est invalide

    float scaleX = destRect.width / (float)texture.width;
    float scaleY = destRect.height / (float)texture.height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY; // On garde le ratio

    float texWidthStr = texture.width * scale;
    float texHeightStr = texture.height * scale;

    // On centre dans le rectangle de destination
    Vector2 pos = { 
        destRect.x + (destRect.width - texWidthStr) / 2.0f, 
        destRect.y + (destRect.height - texHeightStr) / 2.0f 
    };

    DrawTextureEx(texture, pos, 0.0f, scale, WHITE);
}


void chargerTexturesCatalogue(t_catalogue catalogue) {
    nbTexturesChargees = getNbMedia(catalogue);
    if (nbTexturesChargees == 0) return;

    // On alloue de la mémoire pour le tableau de textures
    mesTextures = (Texture2D*)malloc(sizeof(Texture2D) * nbTexturesChargees);

    for (int i = 0; i < nbTexturesChargees; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        char cheminImage[100];
        // On suppose que les images sont des PNG avec le même code que la vidéo
        sprintf(cheminImage, "assets/%s.png", getCode(m));

        if (FileExists(cheminImage)) {
            mesTextures[i] = LoadTexture(cheminImage);
            printf("Texture chargee : %s\n", cheminImage);
        } else {
            // Si l'image n'existe pas, on crée une texture vide "magenta" (couleur d'erreur standard)
            // pour éviter que ça plante.
            Image imgVide = GenImageColor(CARTE_LARGEUR, CARTE_LARGEUR, MAGENTA);
            mesTextures[i] = LoadTextureFromImage(imgVide);
            UnloadImage(imgVide);
            printf("[Avertissement] Image manquante : %s\n", cheminImage);
        }
        // IMPORTANT : On filtre la texture pour qu'elle soit jolie quand on la réduit
        SetTextureFilter(mesTextures[i], TEXTURE_FILTER_BILINEAR);
    }
}

void libererTexturesCatalogue(void) {
    if (mesTextures != NULL) {
        for (int i = 0; i < nbTexturesChargees; i++) {
            UnloadTexture(mesTextures[i]); // Libère la mémoire graphique (VRAM)
        }
        free(mesTextures); // Libère le tableau (RAM)
        mesTextures = NULL;
        nbTexturesChargees = 0;
    }
}

void dessinerFondMenu(void) {
    DrawText("NETFLIX PERSO", 20, 20, 40, RED);
    DrawLine(20, 70, 780, 70, LIGHTGRAY);
}

// Notre nouvelle fonction "Widget" : La Carte Média
int dessinerCarteMedia(Rectangle rect, t_media m, Texture2D miniature) {
    int estClique = 0;
    Vector2 souris = GetMousePosition();
    Color couleurBordure = LIGHTGRAY;

    // 1. Détection Souris (Hover)
    if (CheckCollisionPointRec(souris, rect)) {
        couleurBordure = RED; // Bordure rouge au survol
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            estClique = 1;
        }
    }

    // 2. Dessin du fond de la carte
    DrawRectangleRec(rect, RAYWHITE);

    // 3. Zone pour l'image (on laisse un peu de place pour le texte en bas)
    Rectangle rectImage = { rect.x + 5, rect.y + 5, rect.width - 10, rect.height - 40 };
    
    // On dessine un fond gris sous l'image
    DrawRectangleRec(rectImage, Fade(LIGHTGRAY, 0.3f)); 
    // On dessine l'image redimensionnée
    DrawTextureScaled(miniature, rectImage);
    
    // 4. Zone pour le texte (titre)
    // On coupe le titre s'il est trop long pour la carte
    char titreCoupe[30];
    strncpy(titreCoupe, getTitre(m), 25);
    if(strlen(getTitre(m)) > 25) strcat(titreCoupe, "...");

    DrawText(titreCoupe, rect.x + 5, rect.y + rect.height - 30, 18, DARKGRAY);
    DrawText(TextFormat("(%d)", getAnnee(m)), rect.x + 5, rect.y + rect.height - 12, 10, GRAY);

    // 5. Bordure finale
    DrawRectangleLinesEx(rect, 2, couleurBordure);

    return estClique;
}






void animLogoStart(void)
{
    int logoPositionX = GetScreenWidth()/2 - 128;
    int logoPositionY = GetScreenHeight()/2 - 128;

    int framesCounter = 0;
    int lettersCount = 0;

    int topSideRecWidth = 24;
    int leftSideRecHeight = 24;

    int bottomSideRecWidth = 24;
    int rightSideRecHeight = 24;

    int state = 0;                  // Etat de l'animation
    float alpha = 1.0f;             // Utile pour le fondu

    while (!WindowShouldClose()) 
    {       
        if (state == 0)                 // State 0: Petit carré clignotant
        {
            framesCounter++;
            if (framesCounter == 120)
            {
                state = 1;
                framesCounter = 0;
            }
        }
        else if (state == 1)            // State 1: Barres haut et gauche
        {
            topSideRecWidth += 4;
            leftSideRecHeight += 4;
            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2)            // State 2: Barres bas et droite
        {
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;
            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3)            // State 3: Lettres apparaissent
        {
            framesCounter++;
            if (framesCounter/12)       
            {
                lettersCount++;
                framesCounter = 0;
            }

            // Une fois que tout est affiché (compte large pour NounaFlix)
            if (lettersCount >= 12)     
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    return; // Fin de l'animation
                }
            }
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (state == 0)
            {
                if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            }
            else if (state == 1)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
            }
            else if (state == 2)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
                DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
            }
            else if (state == 3)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

                DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

                DrawText(TextSubtext("NounaFlix", 0, lettersCount), GetScreenWidth()/2 - 84, GetScreenHeight()/2 + 70, 40, Fade(RED, alpha));
            }

        EndDrawing();
    }
}


#define MAX_COLORS_COUNT    21          // Number of colors available

void animCarreMedia(void) {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    Color colors[MAX_COLORS_COUNT] = {
        DARKGRAY, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, PINK, YELLOW,
        GREEN, SKYBLUE, PURPLE, BEIGE };

    const char *colorNames[MAX_COLORS_COUNT] = {
        "DARKGRAY", "MAROON", "ORANGE", "DARKGREEN", "DARKBLUE", "DARKPURPLE",
        "DARKBROWN", "GRAY", "RED", "GOLD", "LIME", "BLUE", "VIOLET", "BROWN",
        "LIGHTGRAY", "PINK", "YELLOW", "GREEN", "SKYBLUE", "PURPLE", "BEIGE" };

    Rectangle colorsRecs[MAX_COLORS_COUNT] = { 0 };     // Rectangles array

    // Fills colorsRecs data (for every rectangle)
    for (int i = 0; i < MAX_COLORS_COUNT; i++)
    {
        colorsRecs[i].x = 20.0f + 100.0f *(i%7) + 10.0f *(i%7);
        colorsRecs[i].y = 80.0f + 100.0f *(i/7) + 10.0f *(i/7);
        colorsRecs[i].width = 100.0f;
        colorsRecs[i].height = 100.0f;
    }

    int colorState[MAX_COLORS_COUNT] = { 0 };           // Color state: 0-DEFAULT, 1-MOUSE_HOVER

    Vector2 mousePoint = { 0.0f, 0.0f };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();

        for (int i = 0; i < MAX_COLORS_COUNT; i++)
        {
            if (CheckCollisionPointRec(mousePoint, colorsRecs[i])) colorState[i] = 1;
            else colorState[i] = 0;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("raylib colors palette", 28, 42, 20, BLACK);
            DrawText("press SPACE to see all colors", GetScreenWidth() - 180, GetScreenHeight() - 40, 10, GRAY);

            for (int i = 0; i < MAX_COLORS_COUNT; i++)    // Draw all rectangles
            {
                DrawRectangleRec(colorsRecs[i], Fade(colors[i], colorState[i]? 0.6f : 1.0f));

                if (IsKeyDown(KEY_SPACE) || colorState[i])
                {
                    DrawRectangle((int)colorsRecs[i].x, (int)(colorsRecs[i].y + colorsRecs[i].height - 26), (int)colorsRecs[i].width, 20, BLACK);
                    DrawRectangleLinesEx(colorsRecs[i], 6, Fade(BLACK, 0.3f));
                    DrawText(colorNames[i], (int)(colorsRecs[i].x + colorsRecs[i].width - MeasureText(colorNames[i], 10) - 12),
                        (int)(colorsRecs[i].y + colorsRecs[i].height - 20), 10, colors[i]);
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

*/

/**
 * Module affichage - affichage.c
 * Par : Corentin Couëron
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "affichage.h"

// --- Variables globales pour stocker les images ---
// IMPORTANT : Pas de "static" ici car on veut l'utiliser dans le main via "extern"
Texture2D* mesTextures = NULL; 
static int nbTexturesChargees = 0;

/* === FONCTIONS DE BASE === */

void initInterface(int largeur, int hauteur, char* titre) {
    InitWindow(largeur, hauteur, titre);
    SetTargetFPS(60);
}

void fermerInterface(void) {
    CloseWindow();
}

/* === GESTION DES TEXTURES (IMAGES) === */

void chargerTexturesCatalogue(t_catalogue catalogue) {
    nbTexturesChargees = getNbMedia(catalogue);
    if (nbTexturesChargees == 0) return;

    // Allocation dynamique du tableau
    mesTextures = (Texture2D*)malloc(sizeof(Texture2D) * nbTexturesChargees);

    for (int i = 0; i < nbTexturesChargees; i++) {
        t_media m = getMediaCatalogue(catalogue, i);
        char cheminImage[100];
        
        // On cherche "assets/MOV001.png"
        sprintf(cheminImage, "assets/%s.png", getCode(m));

        if (FileExists(cheminImage)) {
            mesTextures[i] = LoadTexture(cheminImage);
            // Filtre bilinéaire pour que ce soit joli quand on redimensionne
            SetTextureFilter(mesTextures[i], TEXTURE_FILTER_BILINEAR);
        } else {
            // Image vide (Magenta) si pas trouvée
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

// Fonction utilitaire interne pour redimensionner l'image dans le cadre
void DrawTextureScaled(Texture2D texture, Rectangle destRect) {
    if (texture.id <= 0) return;

    float scaleX = destRect.width / (float)texture.width;
    float scaleY = destRect.height / (float)texture.height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY; 

    float texWidthStr = texture.width * scale;
    float texHeightStr = texture.height * scale;

    Vector2 pos = { 
        destRect.x + (destRect.width - texWidthStr) / 2.0f, 
        destRect.y + (destRect.height - texHeightStr) / 2.0f 
    };

    DrawTextureEx(texture, pos, 0.0f, scale, WHITE);
}

/* === ELEMENTS GRAPHIQUES (WIDGETS) === */

void dessinerFondMenu(void) {
    DrawText("NOUNAFLIX", 20, 20, 40, RED);
    DrawLine(20, 70, GetScreenWidth()-20, 70, LIGHTGRAY);
}

int dessinerCarteMedia(Rectangle rect, t_media m, Texture2D miniature) {
    int estClique = 0;
    Vector2 souris = GetMousePosition();
    Color couleurBordure = LIGHTGRAY;

    // Detection Souris (Hover)
    if (CheckCollisionPointRec(souris, rect)) {
        couleurBordure = RED; 
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            estClique = 1;
        }
    }

    // Fond + Image
    DrawRectangleRec(rect, RAYWHITE);
    
    Rectangle rectImage = { rect.x + 5, rect.y + 5, rect.width - 10, rect.height - 50 };
    DrawRectangleRec(rectImage, Fade(LIGHTGRAY, 0.3f)); 
    DrawTextureScaled(miniature, rectImage);
    
    // Texte (Titre + Année)
    char titreCoupe[30];
    strncpy(titreCoupe, getTitre(m), 18);
    titreCoupe[18] = '\0'; // Securité
    if(strlen(getTitre(m)) > 18) strcat(titreCoupe, "...");

    DrawText(titreCoupe, rect.x + 5, rect.y + rect.height - 40, 18, DARKGRAY);
    DrawText(TextFormat("(%d)", getAnnee(m)), rect.x + 5, rect.y + rect.height - 20, 10, GRAY);

    // Bordure
    DrawRectangleLinesEx(rect, 2, couleurBordure);

    return estClique;
}

/* === ANIMATIONS === */

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
                DrawText(TextSubtext("NounaFlix", 0, lettersCount), GetScreenWidth()/2 - 100, GetScreenHeight()/2 + 40, 40, Fade(RED, alpha));
            }
        EndDrawing();
    }
}

