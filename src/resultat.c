// Fichier: src/resultat.c
#include "resultat.h"
#include <string.h> // <--- TRES IMPORTANT POUR strdup
#include <stdlib.h>
#include <assert.h>

struct media{
    char *code; char *type; char *titre;
    int annee_sortie; float duree; char *auteur;
};

struct resultat {
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

t_resultat creer_resultat(void) {
    t_resultat res = malloc(sizeof(struct resultat));
    assert(res != NULL);
    res->media = NULL; res->nb_media = 0;
    return res;
}

// --- Allocation du tableau (Fonction ajoutée) ---
void allouerTableauMedia(t_resultat res, int taille_max) {
    if (res != NULL) {
        res->media = (t_media*) malloc(taille_max * sizeof(t_media));
        for(int i=0; i<taille_max; i++) res->media[i] = NULL;
    }
}

// --- Getters ---
char* getCode(t_media media) { return (media) ? media->code : NULL; }
char* getType(t_media media) { return (media) ? media->type : NULL; }
char* getTitre(t_media media) { return (media) ? media->titre : NULL; }
int getAnnee(t_media media) { return (media) ? media->annee_sortie : 0; }
float getDuree(t_media media) { return (media) ? media->duree : 0.0; }
char* getAuteur(t_media media) { return (media) ? media->auteur : NULL; }
t_media get_media_resultat(t_resultat resultat, int index) { return resultat->media[index]; }
int get_nb_media(t_resultat resultat) { return resultat->nb_media; }

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

void setNbMedia(t_resultat resultat, int nb_media) {
    resultat->nb_media = nb_media;
}

// Fonction simplifiée pour stocker le média
void setMediaResultat(t_resultat resultat, t_media media, int index) {
    assert(index >= 0);
    // On stocke directement le pointeur créé dans interface.c
    resultat->media[index] = media;
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

void freeResultat(t_resultat resultat) {
    if (!resultat) return;
    for (int i = 0; i < resultat->nb_media; i++) freeMedia(resultat->media[i]);
    free(resultat->media);
    free(resultat);
}