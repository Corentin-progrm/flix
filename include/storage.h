/**
 * Module storage - storage.h
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module storage gère le chargement de la base de données
 *               depuis un fichier texte, la vérification de l'existence des 
 *               fichiers multimédia et la lecture des informations.
**/

#ifndef STORAGE_H
#define STORAGE_H

/* LIBRARY ================================================================ */
#include "modele.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/* DECLARATION DES FONCTIONS ============================================== */

// Charge le fichier texte, vérifie les .mp4, et retourne TOUTE la structure remplie
t_catalogue chargerBaseDeDonnees(void);

// Permet de vérifier si un fichier existe à un chemin donné
int fichierExiste(char *chemin);

// Lance la vidéo associée au media
void lancerVideo(t_media media);

#endif