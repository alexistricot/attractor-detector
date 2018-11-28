#include <stdlib.h>
#include <stdio.h>

#include "init.h"

void initAutomatas(AN *an){
  int i;
  for(i=0;i<an->nbauto;i++){
    an->automatas[i].start = -1;
    an->automatas[i].nbtrans = 0;
  }
  for(i=0;i<an->nbtrans;i++){
    an->automatas[an->transitions[i].automata].nbtrans += 1;
    if(an->automatas[an->transitions[i].automata].start == -1){
      an->automatas[an->transitions[i].automata].start = i;
    }
  }
  // return an;
}
