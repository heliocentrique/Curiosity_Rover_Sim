#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fonction pour vérifier si un terrain est valide (un chemin existe du centre au bord)
int terrain_valide(Terrain *T, int l, int h, float dObst) {
    // Génération d'un terrain avec obstacles
    generation_aleatoire(T, l, h, dObst);

    // Vérification si un chemin existe depuis le centre jusqu'au bord
    return existe_chemin_vers_sortie(T);
}

// Fonction pour générer un terrain valide
void generer_terrain_valide(Terrain *T, int l, int h, float dObst) {
    int max_tries = 100;  // Nombre maximum de tentatives pour générer un terrain valide
    int tries = 0;

    // Repeter la generation du terrain jusqu'a ce qu'il soit valide ou trop de tentatives
    do {
        generation_aleatoire(T, l, h, dObst);
        tries++;
        if (existe_chemin_vers_sortie(T)) {
            return;  // Terrain valide trouver, sortir de la fonction
        }
    } while (tries < max_tries);  // Limiter a 100 tentatives

    // Si on dépasse le nombre de tentatives sans succes, afficher un message d'avertissement
    printf("Avertissement: Terrain invalide apres %d tentatives.\n", max_tries);
}

// Fonction pour compter le nombre de cases occupees
int nb_cases_occupees(Terrain *T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T->tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

int main(int argc, char **argv) {
  int N, i, l, h;
  float dObst;
  FILE *resFile;
  Terrain T;

  // Vérification des arguments passés au programme
  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);  // Nombre de terrains à générer
  l = strtol(argv[2], NULL, 10);  // Largeur du terrain
  h = strtol(argv[3], NULL, 10);  // Hauteur du terrain
  dObst = strtof(argv[4], NULL);  // Densité des obstacles

  // Test de validité des dimensions du terrain
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  if (resFile == NULL) {
    printf("Erreur lors de l'ouverture du fichier %s\n", argv[5]);
    return 1;
  }

  // Écriture du nombre de terrains à générer
  fprintf(resFile, "%d\n", N);

  // Initialisation de la fonction random
  srand(time(NULL));

  // Variables pour calculer les statistiques globales
  int total_cases_occupees = 0;
  int total_valides = 0;

  // Génération aléatoire des terrains
  for (i = 0; i < N; i++) {
    // Générer un terrain valide
    generer_terrain_valide(&T, l, h, dObst);

    // Calcul du nombre de cases occupées
    int cases_occupees = nb_cases_occupees(&T);

    // Calcul de la densité d'obstacle pour ce terrain
    float densite_obstacle = (float)cases_occupees / (l * h);

    // Écrire le terrain dans le fichier
    ecrire_terrain(resFile, &T, l / 2, h / 2);  // Écrire le terrain avec la position du robot au centre

    // Écrire la densité d'obstacle du terrain
    fprintf(resFile, "Densité d'obstacle pour terrain %d: %.2f\n", i + 1, densite_obstacle);

    // Afficher des statistiques sur le terrain
    printf("Terrain %d : %d cases occupées, densité d'obstacle = %.2f\n", i + 1, cases_occupees, densite_obstacle);

    // Ajouter aux statistiques globales
    total_cases_occupees += cases_occupees;

    // Vérifier si le terrain est valide
    if (existe_chemin_vers_sortie(&T)) {
      total_valides++;
    }
  }

  // Calcul de la densité moyenne des obstacles
  float densite_moyenne = (float)total_cases_occupees / (N * l * h);

  // Calcul du pourcentage de terrains valides
  float pourcentage_valides = (float)total_valides / N * 100;

  // Écrire les statistiques finales dans le fichier
  fprintf(resFile, "\nDensité moyenne des obstacles : %.2f\n", densite_moyenne);
  fprintf(resFile, "Pourcentage de terrains valides : %.2f%%\n", pourcentage_valides);

  // Fermeture du fichier de résultats
  fclose(resFile);

  return 0;
}