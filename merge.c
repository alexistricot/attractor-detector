#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "merge.h"

// int countIntersections(AN an, BasinList *bl){
//
//   int n = an.nbauto;
//   int max = 1;
//   int diff;
//   Basin *b = bl->first;
//   AutoList *l;
//   Element *e;
//   int cond[n];
//   int i;
//   for(i = 0; i < n; i++){
//     cond[i] = -1;
//   }
//   while(b != NULL){
//     l = b->autolist;
//     e = l->first;
//     diff = 0;
//     while(e!=NULL){
//       if(cond[e->automata.number] == -1){
//         cond[e->automata.number] = e->state;
//       }
//       if(cond[e->automata.number] > -1 && cond[e->automata.number != e->state]){
//         diff = 1;
//         cond[e->automata.number] = -2;
//       }
//       e = e->next;
//     }
//     if(diff){
//       max += 1;
//     }
//     b = b->next;
//   }
//   return max;
// }

Basin *intersect(AN an, Basin *b1, Basin *b2){

  AutoList *a1 = b1->autolist;
  AutoList *a2 = b2->autolist;
  renumber(an,a1);
  renumber(an,a2);
  int local[an.nbauto]; //local[i] valeur de l'automate i dans a1
  int i;
  for(i = 0; i < an.nbauto; i++){
    local[i] = -1;
  }
  int disjoint = 0;
  Element *e1;
  AutoList *result = NULL;
  Element *e = a1->first;
  while(e!=NULL){
    if(result == NULL){
      e1 = newElement(e->automata,e->state);
      result = newAutoList(e1);
    }
    else{
      push(result, e->automata, e->state);
    }
    local[e->automata.number] = e->state;
    e = e->next;
  }

  e = a2->first;
  while(e!=NULL){
    if(local[e->automata.number]==-1){
      push(result, e->automata, e->state);
    }
    else{
      if(local[e->automata.number] != e->state){
        disjoint = 1;
      }
    }
    e = e->next;
  }
  if(disjoint){
    return NULL;
  }
  else{
    Basin *b = newBasin(result);
    return b;
  }
}

BasinList *merge(an, BasinList *bl){

  int keep;
  Basin *b = bl->first;
  BasinList b1 = 
  while(b != NULL){
    keep = 1;
    Basin *b1 = b->next;
    while(b1 != NULL){
      Basin *inter = intersect(an,b,b1);
      if(inter != NULL){

      }
    }
  }

}
