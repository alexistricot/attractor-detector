#include <stdlib.h>
#include <stdio.h>

#include "automata.h"

void removeAutomata(AN *an,int nb){

  int i;
  int j;
  int k;
  GlobalState g = an->initial;
  int s = g.states[nb];
  if(s == -1){
    printf("Deleting non initialized automata %s",an->automatas[nb].name);
  }
  Transition t;
  int delete[an->nbtrans];
  int nbdelete = 0;
  for(i = 0; i < an->nbtrans; i++){
    delete[i] = -1;
  }
  for(i = nb; i < an->nbauto-1; i++){
    an->automatas[i] = an->automatas[i+1];
    an->automatas[i].number = an->automatas[i].number - 1;
    g.states[i] = g.states[i+1];
  }
  an->nbauto += -1;
  g.nbauto += -1;
  an->initial = g;

  for(i = 0; i < an->nbtrans; i++){
    t = an->transitions[i];
    if(t.automata == nb){
      delete[nbdelete] = i;
      nbdelete += 1;
    }
    else{
      if(t.automata > nb){
        t.automata = t.automata - 1;
      }
      for(j = 0; j < t.nbcond; j++){
        if(t.automCond[j] == nb){
          if(t.stateCond[j] == s){
            for(k = j; k < t.nbcond - 1; k++){
              t.automCond[k] = t.automCond[k+1];
              t.stateCond[k] = t.stateCond[k+1];
            }
            t.nbcond += -1;
          }
          else{
            delete[nbdelete] = i;
            nbdelete += 1;
          }
        }
        if(t.automCond[j] > nb){
          t.automCond[j] = t.automCond[j] - 1;
        }
      }
    }
    an->transitions[i] = t;
  }
  for(i = 0; i < nbdelete; i++){
    // printf("removing transition %d, automata %s, from state %d\n",delete[i],an->automatas[an->transitions[delete[i]].automata].name,an->transitions[delete[i]].fst);
    removeTransition(an,delete[i]-i);
  }
  // return an;
}
