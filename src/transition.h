#ifndef __TRANSITION_H
#define __TRANSITION_H

#include "struct.h"

// ajoute à chaque état local de chaque automate sa fréquence de transition
// remarque : approximmation à l'ordre 4
void countTrans(AN *an);

// enlève la transition numéro nb
void removeTransition(AN *an, int nb);

#endif
