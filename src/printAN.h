#ifndef __PRINTAN_H
#define __PRINTAN_H

#include "struct.h"

// affiche tout le réseau : automates, transitions, état initial, fréquences
void printAN(AN an);

// affiche les automates
void printAutomatas(AN an);

// affiche les transitions
void printTransitions(AN an);

// affiche l'état initial
void printInitial(AN an);

// affiche les fréquences de transition des états locaux
void printFrequency(AN an);

#endif
