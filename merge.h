#ifndef __MERGE_H
#define __MERGE_H

#include "struct.h"
#include "autolists.h"
#include "clique.h"

//renvoie NULL si b1 et b2 sont disjoints
//renvoie l'intersection sinon
Basin *intersect(AN an, Basin *b1, Basin *b2);

// renvoie une matrice m telle que m[i][j] = 1 si
// les bassins i et j sont fusionnables
int **fusionnable(AN an, BasinList *bl);


void bronKerbosch(int *r, int *p, int *x, int *np, int *nx, int n, int **m, CliqueList *cl);

// renvoie l'intersection de px et les voisins de v
int *voisins(int *px, int **m, int n, int v, int *npx);

// renvoie r union {v}
int *rRec(int n, int v, int *r);

//Idée : cette fonction doit fusionner les bassins trouvés afin d'obtenir des bassins minimaux.
//Au 17/09/2018, elle n'est pas terminée.
BasinList *merge(AN an, BasinList *bl);

#endif
