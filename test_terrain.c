#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  erreur_terrain erreur;
  int x, y;

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }

 do {
        f = fopen(argv[1], "r");
        if (f == NULL) {
            printf("Erreur d'ouverture du fichier. Veuillez entrer un nouveau nom de fichier : ");
            scanf("%s", argv[1]);
        }
    } while (f == NULL);

    erreur = lire_terrain(f, &t, &x, &y);
    fclose(f);

    if (erreur != OK) {
        switch (erreur) {
            case ERREUR_FICHIER:
                printf("Erreur d'ouverture du fichier.\n");
                break;
            case ERREUR_LECTURE_LARGEUR:
                printf("Erreur : lecture de la largeur du terrain.\n");
                break;
            case ERREUR_LECTURE_HAUTEUR:
                printf("Erreur : lecture de la hauteur du terrain.\n");
                break;
            case ERREUR_LARGEUR_INCORRECTE:
                printf("Erreur : largeur du terrain incorrecte.\n");
                break;
            case ERREUR_HAUTEUR_INCORRECTE:
                printf("Erreur : hauteur du terrain incorrecte.\n");
                break;
            case ERREUR_LIGNE_TROP_LONGUE:
                printf("Erreur : ligne trop longue dans le terrain.\n");
                break;
            case ERREUR_LIGNE_TROP_COURTE:
                printf("Erreur : ligne trop courte dans le terrain.\n");
                break;
            case ERREUR_LIGNES_MANQUANTES:
                printf("Erreur : lignes manquantes dans le terrain.\n");
                break;
            case ERREUR_POSITION_ROBOT_MANQUANTE:
                printf("Erreur : position du robot manquante.\n");
                break;
            default:
                printf("Erreur inconnue.\n");
        }
        return 1;
    }

    afficher_terrain(&t);
    printf("Position initiale du robot : (%d, %d)\n", x, y);

    return 0;
}
