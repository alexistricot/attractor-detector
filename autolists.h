#ifndef __AUTOLISTS_H
#define __AUTOLISTS_H

#include "struct.h"

typedef struct Element Element;
struct Element {
  Automata automata;
  int state;
  Element *next;
};

typedef struct {
  Element *first;
  int result;
} AutoList;

typedef struct Basin Basin;
struct Basin{
  AutoList *autolist;
  Basin *next;
};

typedef struct{
  Basin *first;
} BasinList;

// crée un élément correspondant à l'état local de l'automate a dans l'état state
Element *newElement(Automata a, int state);

// crée une liste d'états locaux initialisée avec l'élément e
AutoList *newAutoList(Element *e);

// crée un bassin à partir de la liste l
Basin *newBasin(AutoList *l);

// crée une liste de bassins initialisée avec le bassin b
BasinList *newBasinList(Basin *b);

// affiche les états locaux de l
void printList(AutoList *l);

//ajoute l'état local a_state à la liste l
void push(AutoList *l, Automata a, int state);

//ajoute le bassin b à la liste bl
void pushBasin(BasinList *bl, AutoList *l);

// crée une nouvelle liste en concaténant l1 et l2
AutoList *autoListCat(AutoList *l1, AutoList *l2);

// longueur de la liste commençant par e
int length(Element *e);

// vaut 1 si bl contient un bassin correspondant à l, 0 sinon.
// requiert le contexte an
int isIn(AN an, AutoList *l, BasinList *bl);

//libère les éléments suivant e
void freeList(Element *e);

//libère les bassins suivant e
void freeBasin(Basin *e);

//renumérote les automates de l
void renumber(AN an, AutoList *l);

#endif
