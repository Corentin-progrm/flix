/**
 * Module modele - modele.c
 *
 * Par : Corentin Couëron
 * Date : 07-01-2026
 * Description : Le module modele définit les structures de données,
 *               tels que le media qui contient les informations d'un media
 *               et le catalogue qui contient une liste de medias.
 *               Il fournit également des fonctions pour créer, accéder, setter
 *               et libérer ces structures.
**/


/* LIBRARY ================================================================ */
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "modele.h"

/* DEFINITION STRUCTURES ================================================== */
struct media{
    char *code; 
    char *type; 
    char *titre;
    int annee_sortie; 
    int duree; 
    char *auteur;
};

struct catalogue {
    t_media *media;
    int nb_media;
};

/* DEFINTIONS CONSTRUCTEURS =============================================== */

/**
* @fonction creer_media()
* @brief Permet de créer et initialiser une nouvelle instance de t_media.
* @return media     Le nouveau media créé.
*/
t_media creer_media(void) {
    t_media media = (t_media) malloc(sizeof(struct media));     // Allocation mémoire
    assert(media != NULL);          // Vérification allocation
    
    // Initialisation des champs
    if (media) {
        media->code = NULL; 
        media->type = NULL; 
        media->titre = NULL;
        media->auteur = NULL;
        media->annee_sortie = 0; 
        media->duree = 0;
    }
    
    return media;
}

/**
 * @fonction creer_catalogue()
 * @brief Permet de créer et initialiser une nouvelle instance de t_catalogue.
 * @return catalogue     Le nouveau catalogue créé.
 */
t_catalogue creer_catalogue(void) {
    t_catalogue catalogue = malloc(sizeof(struct catalogue));   // Allocation mémoire
    assert(catalogue != NULL);    // Vérification allocation
    
    // Initialisation des champs
    if (catalogue) {
        catalogue->media = NULL; 
        catalogue->nb_media = 0;
    }
    
    return catalogue;
}

/* MEMOIRES =============================================================== */

/**
* @fonction allouerTableauMedia()
* @brief Alloue la mémoire pour le tableau de medias dans le catalogue.
* @param  catalogue     Le catalogue dans lequel allouer le tableau.
* @param  taille_max    La taille maximale du tableau de medias.
*/
void allouerTableauMedia(t_catalogue catalogue, int taille_max) {
    if (catalogue != NULL) {
        catalogue->media = (t_media*) malloc(taille_max * sizeof(t_media));
        
        // Initialisation des pointeurs à NULL
        for (int i = 0; i < taille_max; i++) {
            catalogue->media[i] = NULL;
        }
    }
}

/* GETTERS ================================================================ */
/**
 * @fonction getCode()
 * @brief Retourne le code du media.
 * @param  media    Le media dont on veut obtenir le code.
 * @return char*    Le code du media.
 */
char* getCode(t_media media) {
    if (media != NULL) {
        return media->code;
    } else {
        return NULL;
    }
}

/**
 * @fonction getType()
 * @brief Retourne le type du media.
 * @param  media    Le media dont on veut obtenir le type.
 * @return char*    Le type du media.
 */
char* getType(t_media media) {
    if (media != NULL) {
        return media->type;
    } else {
        return NULL;
    }
}

/**
 * @fonction getTitre()
 * @brief Retourne le titre du media.
 * @param  media    Le media dont on veut obtenir le titre.
 * @return char*    Le titre du media.
 */
char* getTitre(t_media media) {
    if (media != NULL) {
        return media->titre;
    } else {
        return NULL;
    }
}

/**
 * @fonction getAnnee()
 * @brief Retourne l'année de sortie du media.
 * @param  media    Le media dont on veut obtenir l'année de sortie.
 * @return int      L'année de sortie du media.
 */
int getAnnee(t_media media) {
    if (media) {
        return media->annee_sortie;
    } else {
        return 0;
    }
}

/**
 * @fonction getDuree()
 * @brief Retourne la durée du media en minutes.
 * @param  media    Le media dont on veut obtenir la durée.
 * @return int      La durée du media en minutes.
 */
int getDuree(t_media media) {
    if (media) {
        return media->duree;
    } else {
        return 0;
    }
}

/**
 * @fonction getAuteur()
 * @brief Retourne l'auteur du media.
 * @param  media    Le media dont on veut obtenir l'auteur.
 * @return char*    L'auteur du media.
 */
char* getAuteur(t_media media) {
    if (media) {
        return media->auteur;
    } else {
        return NULL;
    }
}

/**
 * @fonction get_media_catalogue()
 * @brief Retourne le media à l'index donné dans le catalogue.
 * @param  catalogue    Le catalogue contenant les medias.
 * @param  index        L'index du media à récupérer.
 * @return t_media      Le media à l'index donné.
 */
t_media getMediaCatalogue(t_catalogue catalogue, int index) {
    return catalogue->media[index];
}

/**
 * @fonction get_nb_media()
 * @brief Retourne le nombre de medias dans le catalogue.
 * @param  catalogue    Le catalogue dont on veut obtenir le nombre de medias.
 * @return int          Le nombre de medias dans le catalogue.
 */
int getNbMedia(t_catalogue catalogue) {
    return catalogue->nb_media;
}

/* SETTERS ================================================================ */
/**
 * @fonction setCode()
 * @brief Définit le code du media.
 * @param  media         Le media dont on veut définir le code.
 * @param  nouveauCode   Le nouveau code à définir.
 */
void setCode(t_media media, char* nouveauCode) {
    if (media->code != NULL) {
        free(media->code);
    }
    media->code = strdup(nouveauCode);
}

/**
 * @fonction setType()
 * @brief Définit le type du media.
 * @param  media         Le media dont on veut définir le type.
 * @param  nouveauType   Le nouveau type à définir.
 */
void setType(t_media media, char* nouveauType) {
    if (media->type != NULL) {
        free(media->type);
    }
    media->type = strdup(nouveauType);
}

/**
 * @fonction setTitre()
 * @brief Définit le titre du media.
 * @param  media         Le media dont on veut définir le titre.
 * @param  nouveauTitre  Le nouveau titre à définir.
 */
void setTitre(t_media media, char* nouveauTitre) {
    if (media->titre != NULL) {
        free(media->titre);
    }
    media->titre = strdup(nouveauTitre);
}

/**
 * @fonction setAuteur()
 * @brief Définit l'auteur du media.
 * @param  media         Le media dont on veut définir l'auteur.
 * @param  nouvelAuteur  Le nouvel auteur à définir.
 */
void setAuteur(t_media media, char* nouvelAuteur) {
    if (media->auteur != NULL) {
        free(media->auteur);
    }
    media->auteur = strdup(nouvelAuteur);
}

/**
 * @fonction setAnnee()
 * @brief Définit l'année de sortie du media.
 * @param  media          Le media dont on veut définir l'année de sortie.
 * @param  nouvelleAnnee  La nouvelle année de sortie à définir.
 */
void setAnnee(t_media media, int nouvelleAnnee) {
    media->annee_sortie = nouvelleAnnee;
}

/**
 * @fonction setDuree()
 * @brief Définit la durée du media en minutes.
 * @param  media          Le media dont on veut définir la durée.
 * @param  nouvelleDuree  La nouvelle durée à définir en minutes.
 */
void setDuree(t_media media, int nouvelleDuree) {
    media->duree = nouvelleDuree;
}

/**
 * @fonction setNbMedia()
 * @brief Définit le nombre de medias dans le catalogue.
 * @param  catalogue     Le catalogue dont on veut définir le nombre de medias.
 * @param  nb_media      Le nouveau nombre de medias à définir.
 */
void setNbMedia(t_catalogue catalogue, int nb_media) {
    catalogue->nb_media = nb_media;
}

/**
 * @fonction setMediaCatalogue()
 * @brief Stocke un media à l'index donné dans le catalogue.
 * @param  catalogue     Le catalogue dans lequel on veut stocker le media.
 * @param  media         Le media à stocker.
 * @param  index         L'index où on veut stocker le media.
 */
void setMediaCatalogue(t_catalogue catalogue, t_media media, int index) {
    assert(index >= 0);    // Sécurité index
    catalogue->media[index] = media;
}

/* DESTRUCTEURS =========================================================== */

/**
 * @fonction freeMedia()
 * @brief Libère la mémoire allouée pour un media.
 * @param  media    Le media à libérer.
 */
void freeMedia(t_media media) {
    if (!media) return;
    if (media->code) free(media->code);
    if (media->type) free(media->type);
    if (media->titre) free(media->titre);
    if (media->auteur) free(media->auteur);
    free(media);
}

/**
 * @fonction freeCatalogue()
 * @brief Libère la mémoire allouée pour un catalogue et ses medias.
 * @param  catalogue    Le catalogue à libérer.
 */
void freeCatalogue(t_catalogue catalogue) {
    if (catalogue == NULL) {
        return;
    }

    // Libération de chaque media dans le catalogue
    for (int i = 0; i < catalogue->nb_media; i++) {
        freeMedia(catalogue->media[i]);
    }

    free(catalogue->media);
    free(catalogue);
}