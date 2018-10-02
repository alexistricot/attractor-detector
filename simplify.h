#ifndef __SIMPLIFY_H
#define __SIMPLIFY_H

#include "struct.h"
#include "init.h"
#include "automata.h"

// simplifie les transitions de an
void simplifyTrans(AN *an);

//simplifie le réseau en fonction de l'état initial
void simplifyAN(AN *an);

int min(int a, int b);

#endif
