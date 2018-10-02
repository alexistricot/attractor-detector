#ifndef __ATTRACTOR_H
#define __ATTRACTOR_H

#include "struct.h"
#include "autolists.h"
#include "frequency.h"
#include "automata.h"
#include "simplify.h"
#include "printAN.h"

// fonction de recherche d'un bassin dans le réseau d'automate an.
AutoList* findBasin(AN an);

// fonction de recherche de sous-bassin dans le bassin représenté par l.
// an0 réseau d'automates
// l liste représentant le sur-bassin dans lequel on recherche
// prev liste des automates ayant déjà été utilisés comme conditions initiales
AutoList *basinFromBasin(AN an0, AutoList *cond, AutoList *prev);

#endif
