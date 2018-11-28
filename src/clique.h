#ifndef __CLIQUE_H
#define __CLIQUE_H

typedef struct Clique Clique;
struct Clique {
  int *vertices;
  Clique *next;
};

typedef struct {
  Clique *first;
} CliqueList;

// Crée une clique du tableau t
Clique *newClique(int *t);

// Crée une CliqueList initialisée par la clique c
CliqueList *newCliqueList(Clique *c);

void freeCliqueList(Clique *c);

// longueur de la CliqueList
int cliqueLength(CliqueList *cl);

int cliqueLengthAux(Clique *c);

// ajoute une clique
void pushClique(CliqueList *cl, int *t);

// affiche le contenu de la clique c
void printClique(int *t, int n);

#endif
