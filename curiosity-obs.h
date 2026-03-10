#ifndef _CURIOSITY_OBS_H_
#define _CURIOSITY_OBS_H_

#include "observateur.h"
#include "environnement.h"

/* Taille maximale pour le programme du robot */
#define MAX_COMMANDES 1000

/**
 * Lire les commandes du programme-robot depuis un fichier.
 * 
 * @param nom_fichier Nom du fichier contenant le programme.
 * @param commandes Tableau où stocker les commandes lues.
 * @param nb_commandes Adresse d'un entier pour stocker le nombre de commandes lues.
 * @return 0 si la lecture est réussie, 1 sinon.
 */
int lire_programme_robot(const char *nom_fichier, Alphabet *commandes, int *nb_commandes);

#endif // _CURIOSITY_OBS_H_
