// Fichier: src/modele.c
#include "modele.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

struct media{
    char *code; char *type; char *titre;
    int annee_sortie; float duree; char *auteur;
};

struct catalogue {
    t_media *media;
    int nb_media;
};

// --- Constructeurs ---
t_media creer_media(void) {
    t_media media = (t_media) malloc(sizeof(struct media));
    assert(media != NULL);
    if (media) {
        media->code = NULL; media->type = NULL; media->titre = NULL;
        media->auteur = NULL;
        media->annee_sortie = 0; media->duree = 0.0;
    }
    return media;
}

t_catalogue creer_catalogue(void) {
    t_catalogue catalogue = malloc(sizeof(struct catalogue));
    assert(catalogue != NULL);
    catalogue->media = NULL; catalogue->nb_media = 0;
    return catalogue;
}

// --- Allocation du tableau (Fonction ajoutée) ---
void allouerTableauMedia(t_catalogue catalogue, int taille_max) {
    if (catalogue != NULL) {
        catalogue->media = (t_media*) malloc(taille_max * sizeof(t_media));
        for(int i=0; i<taille_max; i++) catalogue->media[i] = NULL;
    }
}

// --- Getters ---
char* getCode(t_media media) { return (media) ? media->code : NULL; }
char* getType(t_media media) { return (media) ? media->type : NULL; }
char* getTitre(t_media media) { return (media) ? media->titre : NULL; }
int getAnnee(t_media media) { return (media) ? media->annee_sortie : 0; }
float getDuree(t_media media) { return (media) ? media->duree : 0.0; }
char* getAuteur(t_media media) { return (media) ? media->auteur : NULL; }
t_media get_media_catalogue(t_catalogue catalogue, int index) { return catalogue->media[index]; }
int get_nb_media(t_catalogue catalogue) { return catalogue->nb_media; }

// --- SETTERS CORRIGÉS (C'EST ICI QUE TU DOIS CHANGER) ---
// On utilise strdup() pour créer une VRAIE COPIE INDÉPENDANTE du texte

void setCode(t_media media, char* nouveauCode) {
    if (media->code) free(media->code); // On nettoie l'avant
    media->code = strdup(nouveauCode);  // On clone le nouveau !
}

void setType(t_media media, char* nouveauType) {
    if (media->type) free(media->type);
    media->type = strdup(nouveauType);
}

void setTitre(t_media media, char* nouveauTitre) {
    if (media->titre) free(media->titre);
    media->titre = strdup(nouveauTitre);
}

void setAuteur(t_media media, char* nouvelAuteur) {
    if (media->auteur) free(media->auteur);
    media->auteur = strdup(nouvelAuteur);
}

void setAnnee(t_media media, int nouvelleAnnee) {
    media->annee_sortie = nouvelleAnnee;
}

void setDuree(t_media media, float nouvelleDuree) {
    media->duree = nouvelleDuree;
}

void setNbMedia(t_catalogue catalogue, int nb_media) {
    catalogue->nb_media = nb_media;
}

// Fonction simplifiée pour stocker le média
void setMediaCatalogue(t_catalogue catalogue, t_media media, int index) {
    assert(index >= 0);
    // On stocke directement le pointeur créé dans interface.c
    catalogue->media[index] = media;
}

// --- Destructeurs ---
void freeMedia(t_media media) {
    if (!media) return;
    if (media->code) free(media->code);
    if (media->type) free(media->type);
    if (media->titre) free(media->titre);
    if (media->auteur) free(media->auteur);
    free(media);
}

void freeCatalogue(t_catalogue catalogue) {
    if (!catalogue) return;
    for (int i = 0; i < catalogue->nb_media; i++) freeMedia(catalogue->media[i]);
    free(catalogue->media);
    free(catalogue);
}