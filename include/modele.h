#ifndef MODELE_H
#define MODELE_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

// Declaration de la structure Media
typedef struct media* t_media;
typedef struct catalogue* t_catalogue;

// Constructeur de Media
t_media creer_media(void);
t_catalogue creer_catalogue(void);

// Getters (Pour accéder aux valeurs)
char* getCode(t_media media);
char* getType(t_media media);
char* getTitre(t_media media);
int getAnnee(t_media media);
float getDuree(t_media media);
char* getAuteur(t_media media);

int get_nb_media(t_catalogue catalogue);
t_media get_media_catalogue(t_catalogue catalogue, int index);

// Setter (Pour modifier les valeurs)
void setCode(t_media media, char* nouveauCode);
void setType(t_media media, char* nouveauType);
void setTitre(t_media media, char* nouveauTitre);
void setAnnee(t_media media, int nouvelleAnnee);
void setDuree(t_media media, float nouvelleDuree);
void setAuteur(t_media media, char* nouvelAuteur);
void setNbMedia(t_catalogue catalogue, int nouveauNbMedia);
void setMediaCatalogue(t_catalogue catalogue, t_media media, int index);

// Alloue la mémoire pour le tableau de pointeurs dans resultat
void allouerTableauMedia(t_catalogue catalogue, int taille_max);
// Destructeur de Media
void freeMedia(t_media media);
void freeCatalogue(t_catalogue catalogue);
#endif