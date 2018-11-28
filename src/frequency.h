#ifndef __FREQUENCY_H
#define __FREQUENCY_H

#include "struct.h"

// renvoie l'automate et son état local qui minimise la fréquence de transition
minFrequency minfreq(AN an);

// renvoie la fréquence de transition de l'état local automata_state dans les conditions cond.
double freqLocal(AN an, int automata, int state, int *cond);

#endif
