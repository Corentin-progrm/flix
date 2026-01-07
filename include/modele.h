/**
 * Module modele - modele.h
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module modele définit les structures de données,
 *               tels que le media qui contient les informations d'un media
 *               et le catalogue qui contient une liste de medias.
 *               Il fournit également des fonctions pour créer, accéder, setter
 *               et libérer ces structures.
**/

#ifndef MODELE_H
#define MODELE_H


/* LIBRARY ================================================================ */
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/* DECLARATION STRUCTURES ================================================= */
typedef struct media* t_media;
typedef struct catalogue* t_catalogue;

/* CONSTRUCTEUR =========================================================== */
t_media creer_media(void);
t_catalogue creer_catalogue(void);

/* GETTERS ================================================================ */
char* getCode(t_media media);       // Ex: "MOV001"
char* getType(t_media media);       // Ex: "Film", "Serie", etc.
char* getTitre(t_media media);      // Ex: "Inception"
int getAnnee(t_media media);        // Ex: 2010
int getDuree(t_media media);        // Ex: 148 (minutes)
char* getAuteur(t_media media);     // Ex: "Christopher Nolan"

int getNbMedia(t_catalogue catalogue);    // Nombre de medias dans le catalogue

t_media getMediaCatalogue(t_catalogue catalogue, int index);  // Accès à un media par index

/* SETTERS ================================================================ */
void setCode(t_media media, char* nouveauCode);
void setType(t_media media, char* nouveauType);
void setTitre(t_media media, char* nouveauTitre);
void setAnnee(t_media media, int nouvelleAnnee);
void setDuree(t_media media, int nouvelleDuree);
void setAuteur(t_media media, char* nouvelAuteur);

void setNbMedia(t_catalogue catalogue, int nouveauNbMedia);
void setMediaCatalogue(t_catalogue catalogue, t_media media, int index);    // Stocke un media à l'index donné

/* MÉMOIRE ================================================================ */
void allouerTableauMedia(t_catalogue catalogue, int taille_max);    // Alloue le tableau de medias dans le catalogue

/* DESTRUCTEUR ============================================================ */
void freeMedia(t_media media);
void freeCatalogue(t_catalogue catalogue);


#endif