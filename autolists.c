#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "autolists.h"

Element *newElement(Automata a, int state){

  Element *e = malloc(sizeof(*e));
  e->next = NULL;
  e->automata = a;
  e->state = state;

  return e;

}

AutoList *newAutoList(Element *e){

  AutoList *l = malloc(sizeof(*l));

  l->first = e;
  l->result = 0;

  return l;

}

Basin *newBasin(AutoList *l){

  Basin *b = malloc(sizeof(*b));
  b->next = NULL;
  b->autolist = l;

}

BasinList *newBasinList(Basin *b){

  BasinList *bl = malloc(sizeof(*bl));
  bl->first = b;

}

void push(AutoList *l, Automata a, int state){

  Element *e = l->first;
  while(e->next != NULL){
    e = e->next;
  }
  e->next = newElement(a,state);

}

void pushBasin(BasinList *bl, AutoList *l){
  Basin *b = bl->first;
  while(b->next!=NULL){
    b = b->next;
  }
  b->next = newBasin(l);
}

void printList(AutoList *l){

  Element *e = l->first;

  printf("%s %d",e->automata.name, e->state);
  while(e->next != NULL){
    e = e->next;
    printf(" -> %s %d",e->automata.name,e->state);
  }
  printf("\n");
}

AutoList *autoListCat(AutoList *l1, AutoList *l2){

  if(l1->first == NULL){
    Element *e = newElement(l2->first->automata,l2->first->state);
    AutoList *l = newAutoList(e);
    Element *e1 = l2->first->next;
    while(e1 != NULL){
      push(l,e1->automata,e1->state);
      e1 = e1->next;
    }
    return l;
  }
  else{
    Element *e = newElement(l1->first->automata,l1->first->state);
    AutoList *l = newAutoList(e);
    Element *e1 = l1->first->next;
    while(e1 != NULL){
      push(l,e1->automata,e1->state);
      e1 = e1->next;
    }
    e1 = l2->first;
    while(e1 != NULL){
      push(l,e1->automata,e1->state);
      e1 = e1->next;
    }
    return l;
  }
}

int length(Element *e){

  if(e == NULL){
    return 0;
  }
  else{
    return length(e->next) + 1;
  }
}

int isIn(AN an, AutoList *l, BasinList *bl){

  int n = an.nbauto;
  int i;
  int result = 0;
  int local = 1;
  Basin *b = bl->first;
  AutoList *lb;
  int condl[n];
  for(i = 0; i < n; i++){
    condl[i] = -1;
  }
  Element *e = l->first;
  while(e != NULL){
    for(i = 0; i < n; i++){
      if(strcmp(e->automata.name,an.automatas[i].name)==0){
        condl[i] = e->state;
      }
    }
    e = e->next;
  }
  int condlb[n];
  while(b != NULL){
    for(i = 0; i < n; i++){
      condlb[i] = -1;
    }
    lb = b->autolist;
    e = lb->first;
    while(e != NULL){
      for(i = 0; i < n; i++){
        if(strcmp(e->automata.name,an.automatas[i].name)==0){
          condlb[i] = e->state;
        }
      }
      e = e->next;
    }
    local =1;
    for(i=0; i < n; i++){
      if(condl[i] != condlb[i]){
        local = 0;
      }
      condlb[i] = 0; //réinitialiser pour la prochaine recherche
    }
    if(local){
      result = 1;
    }
    b = b->next;
  }
  return result;
}

void freeList(Element *e){

  if(e->next == NULL){
    free(e);
  }
  else{
    freeList(e->next);
    free(e);
  }

}

void freeBasin(Basin *e){

  if(e->next == NULL){
    free(e);
  }
  else{
    freeBasin(e->next);
    free(e);
  }

}

void renumber(AN an, AutoList *l){

  int i;
  Element *e = l->first;
  while(e!=NULL){
    for(i = 0; i < an.nbauto; i++){
      if(strcmp(e->automata.name,an.automatas[i].name)==0){
        e->automata.number = i;
      }
    }
    e = e->next;
  }
}
