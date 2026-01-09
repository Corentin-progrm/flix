/**
 * Module utils - utils.h
 *
 * Par : Corentin Couëron
 * Date : 08-01-2026
 * Description : Le module utils fournit des fonctions utilitaires
 *               pour des opérations courantes.
**/

/* LIBRARY ================================================================ */
#include "utils.h"
#include <ctype.h>

/* DEFINITION DES FONCTIONS =============================================== */

/**
 * @fonction mettreEnMinuscule()
 * @brief Transforme une chaine en minuscules directement.
 * @param  chaine    La chaine à transformer.
 */
void mettreEnMinuscule(char *chaine) {
    if (!chaine) return;

    // On parcourt la chaine caractère par caractère
    for (int i = 0; chaine[i] != '\0'; i++) {
        chaine[i] = tolower(chaine[i]);
    }
}

/**
 * @fonction estNumerique()
 * @brief Vérifie si une chaine contient uniquement des chiffres.
 * @param  chaine    La chaine à vérifier.
 * @return int       1 si la chaine est numérique, 0 sinon.
 */
int estNumerique(char *chaine) {
    if (chaine == NULL || strlen(chaine) == 0) return 0; // Faux

    for (int i = 0; chaine[i] != '\0'; i++) {
        // Si un seul caractère n'est pas un chiffre, ce n'est pas un nombre
        if (!isdigit(chaine[i])) {
            return 0; // Faux
        }
    }
    return 1; // Vrai
}

/**
 * @fonction convertirEnEntier()
 * @brief Convertit une chaine "123" en entier 123 (Wrapper de atoi).
 * @param  chaine    La chaine à convertir.
 * @return int       La valeur entière convertie.
 */
int convertirEnEntier(char *chaine) {
    if (estNumerique(chaine) == 1){
        return atoi(chaine);
    }
    return 0;
}