#include <stdlib.h>
#include <stdio.h>
#include "clique.h"

Clique *newClique(int *t){

  Clique *c = malloc(sizeof(*c));
  c->next = NULL;
  c->vertices = t;

  return c;

}

CliqueList *newCliqueList(Clique *c){

  CliqueList *cl = malloc(sizeof(*cl));
  cl->first = c;

  return cl;

}

void freeCliqueList(Clique *c){

  if(c->next==NULL){
    free(c);
  }
  else{
    freeCliqueList(c->next);
    free(c);
  }

}

int cliqueLength(CliqueList *cl){
  cliqueLengthAux(cl->first);
}

int cliqueLengthAux(Clique *c){
  if(c == NULL){
    return 0;
  }
  else{
    return 1 + cliqueLengthAux(c->next);
  }
}

void pushClique(CliqueList *cl, int *t){
  Clique *c = cl->first;
  if(c == NULL){
    cl->first = newClique(t);
  }
  else{
    while(c->next != NULL){
      c = c->next;
    }
    c->next = newClique(t);
  }
}

void printClique(int *t, int n){
  int i;
  for(i = 0; i < n; i++){
    printf("%d\t",t[i]);
  }
  printf("\n");
}
