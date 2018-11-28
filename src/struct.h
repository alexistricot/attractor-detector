#ifndef __STRUCT_H
#define __STRUCT_H

#define MAX_LENGTH 1500
#define MAX_STATES 4
#define MAX_AUTOMATAS 120
#define MAX_TRANSITIONS 400
#define MAX_CONDITIONS 12

// Structure des données

// Automate
typedef struct {
  char name[20]; //nom de l'automate
  int number; //numéro de l'automate (attention : change quand on en retire un)
  int nbstates; //nombre d'états de l'automate
  int start; //position de la première transition concernant l'automate
  int nbtrans; // nombre de transitions concernant l'automate
  int states[MAX_STATES]; // états locaux de l'automate
  double frequency[MAX_STATES]; // fréquences de transition des états locaux
} Automata;

// Transition
typedef struct {
  int automata; //automate concerné par la transition
  int fst; //état de départ
  int snd; // état d'arrivée
  int nbcond; // nombre de conditions
  int automCond[MAX_CONDITIONS]; // automates des conditions
  int stateCond[MAX_CONDITIONS]; // états locaux correspondants
} Transition;

// Etat global. Utilisé uniquement pour représenter l'état initial
typedef struct {
  int states[MAX_AUTOMATAS]; // états locaux des automates
  int nbauto; // nombre d'automates
} GlobalState; // sert pour l'état initial mais représente un état global

// Réseau d'automates
typedef struct {
  int nbauto; //nombre d'automates
  Automata automatas[MAX_AUTOMATAS]; //automates
  int nbtrans; //nombre de transitions
  Transition transitions[MAX_TRANSITIONS]; // transitions
  GlobalState initial; //état initial
} AN;

// Structure représentant un automate dans un état local
// Utilisé pour représenter la transition ayant la plus faible fréquence
typedef struct {
  int automata; //automate
  int state; // état local
  double frequency; //fréquence
} minFrequency; // état local et sa fréquence

#endif
