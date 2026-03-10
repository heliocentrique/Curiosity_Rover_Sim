#include "observateur.h"

Etat initial() {return Init;}

Etat transition(Etat e, Alphabet c) {
    switch (e) {
        case Init:
            switch (c){
                case A:
                    return Erreur;

                case G:
                case D:
                    return Init;
                case M:
                    return  Mes;
                default: 
                    return Erreur;

            }
        case Mes: 
            switch (c){
                case A:
                case G:
                case D:
                    return Init;
                case M:
                    return  Mes;   
                default: 
                    return Erreur;
            }
        case Erreur:
            return Erreur;
        default:
            return Erreur;
    }

}

int est_final(Etat e) { return e != Erreur;}