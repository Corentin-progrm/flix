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
#include <ctype.h>

/* DECLARATION DES FONCTIONS ============================================== */

// Charge le fichier texte, vérifie les .mp4, et retourne TOUTE la structure remplie
t_catalogue chargerBaseDeDonnees(void);

// Permet de vérifier si un fichier existe à un chemin donné
int fichierExiste(char *chemin);

// Lance la vidéo associée au media
void lancerVideo(t_media media);

// Vérifie si un media correspond à une catégorie donnée (indexMenu)
int mediaCorrespondCategorie(t_media m, int indexMenu);

// Vérifie si le titre du media contient le texte de recherche (case insensitive)
int texteContient(const char* haystack, const char* needle);

// Fonction appelée par l'affichage pour filtrer
int mediaCorrespondRecherche(t_media m, char* texteRecherche);

// Récupère des suggestions basées sur l'auteur du media donné
int recupererSuggestions(t_catalogue catalogue, t_media m, t_media suggestions[5]);

// Récupère tous les épisodes d'une série donnée par son code parent
int recupererEpisodesSerie(t_catalogue catalogue, char* codeParent, t_media episodes[100]);

#endif