#include "generation_terrains.h"
#include "interprete.h"
#include <stdio.h>
#include <stdlib.h>

/* Gestion des erreurs liées au terrain */
void gestion_erreur_terrain(erreur_terrain e) {
    switch (e) {
    case OK:
        break;
    case ERREUR_FICHIER:
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier terrain.\n");
        exit(1);
    case ERREUR_POSITION_ROBOT_MANQUANTE:
        fprintf(stderr, "Erreur : Position initiale du robot manquante.\n");
        exit(1);
    default:
        fprintf(stderr, "Erreur inconnue liée au terrain.\n");
        exit(1);
    }
}

/* Gestion des erreurs liées au programme robot */
void gestion_erreur_programme(erreur_programme e) {
    switch (e.type_err) {
    case OK_PROGRAMME:
        break;
    case ERREUR_FICHIER_PROGRAMME:
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier programme.\n");
        exit(2);
    case ERREUR_COMMANDE_INCORRECTE:
        fprintf(stderr, "Erreur : Commande incorrecte détectée dans le programme.\n");
        exit(2);
    default:
        fprintf(stderr, "Erreur inconnue dans le programme robot.\n");
        exit(2);
    }
}

int main(int argc, char **argv) {
    if (argc != 9) {
        fprintf(stderr,
                "Usage : %s <programme> <N> <L> <H> <d> <graine> <nb_steps_max> <fichier_res>\n",
                argv[0]);
        return 1;
    }

    /* Récupérer les arguments */
    char *fichier_programme = argv[1];
    int N = atoi(argv[2]);        // Nombre de terrains
    int L = atoi(argv[3]);        // Largeur des terrains
    int H = atoi(argv[4]);        // Hauteur des terrains
    float d = atof(argv[5]);      // Densité d'obstacles
    int graine = atoi(argv[6]);   // Graine pour la génération aléatoire
    int nb_steps_max = atoi(argv[7]); // Nombre maximum de pas
    char *fichier_res = argv[8];  // Fichier de résultats

    if (L % 2 == 0 || H % 2 == 0) {
        fprintf(stderr, "Erreur : Les dimensions L et H doivent être impaires.\n");
        return 1;
    }

    /* Variables pour les statistiques */
    int sorties = 0, bloque = 0, obstacles = 0, tombe_eau = 0, total_steps = 0;

    /* Initialisation du générateur aléatoire */
    srand(graine);

    FILE *res_file = fopen(fichier_res, "w");
    if (!res_file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier de résultats.\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        /* Génération du terrain */
        Terrain terrain;
        do {
            generation_aleatoire(&terrain, L, H, d);
        } while (!existe_chemin_vers_sortie(&terrain));

        /* Initialisation de l'environnement */
        Environnement envt;
        envt.t = terrain;
        envt.r.x = L / 2;
        envt.r.y = H / 2;
        envt.r.o = Nord;

        /* Charger le programme robot */
        Programme prog;
        erreur_programme err_prog = lire_programme(&prog, fichier_programme);
        gestion_erreur_programme(err_prog);

        /* Exécution du programme */
        etat_inter etat;
        init_etat(&etat);
        resultat_inter res = OK_ROBOT;
        int steps = 0;

        for (; steps < nb_steps_max && res == OK_ROBOT; steps++) {
            res = exec_pas(&prog, &envt, &etat);
        }

        /* Analyse des résultats */
        switch (res) {
        case SORTIE_ROBOT:
            fprintf(res_file, "%d\n", steps);
            sorties++;
            total_steps += steps;
            break;
        case PLOUF_ROBOT:
            fprintf(res_file, "-2\n");
            tombe_eau++;
            break;
        case CRASH_ROBOT:
            fprintf(res_file, "-3\n");
            obstacles++;
            break;
        case OK_ROBOT:
            fprintf(res_file, "-1\n");
            bloque++;
            break;
        default:
            fprintf(stderr, "Erreur inconnue lors de l'exécution.\n");
            fclose(res_file);
            return 1;
        }
    }

    /* Affichage des statistiques */
    printf("Statistiques :\n");
    printf("  Terrains testés : %d\n", N);
    printf("  Sorties réussies : %d (%.2f%%)\n", sorties, (sorties * 100.0) / N);
    printf("  Bloqué : %d (%.2f%%)\n", bloque, (bloque * 100.0) / N);
    printf("  Tombé dans l'eau : %d (%.2f%%)\n", tombe_eau, (tombe_eau * 100.0) / N);
    printf("  Obstacles rencontrés : %d (%.2f%%)\n", obstacles, (obstacles * 100.0) / N);
    if (sorties > 0) {
        printf("  Nombre moyen de pas pour les sorties : %.2f\n", total_steps / (float)sorties);
    }

    fclose(res_file);
    return 0;
}