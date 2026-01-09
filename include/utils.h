/**
 * Module utils - utils.h
 *
 * Par : Corentin Couëron
 * Date : 08-01-2026
 * Description : Le module utils fournit des fonctions utilitaires
 *               pour des opérations courantes.
**/

#ifndef UTILS_H
#define UTILS_H

/* LIBRARY ================================================================ */
#include <string.h>
#include <stdlib.h>

/* DECLARATION DES FONCTIONS ============================================== */

// Transforme une chaine en minuscules directement
void mettreEnMinuscule(char *chaine);

// Vérifie si une chaine contient uniquement des chiffres
int estNumerique(char *chaine);

// Convertit une chaine "123" en entier 123 (Wrapper de atoi)
int convertirEnEntier(char *chaine);

#endif