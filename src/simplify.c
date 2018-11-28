#include <stdlib.h>
#include <stdio.h>

#include "simplify.h"

void simplifyTrans(AN *an){

  int i;
  int s;
  int k;
  int l;
  int m;
  int id;
  int restart;
  Automata a;
  Transition t0;
  Transition t;
  for(i = 0; i<an->nbauto;i++){
    int j;
    restart = 1;
    while(restart){
      initAutomatas(an);
      a = an->automatas[i];
      int delete[a.nbtrans];
      for(m = 0; m < a.nbtrans; m++){
        delete[m] = 0;
      }
      int nbdelete = 0;
      for(j=a.start; j<a.start + a.nbtrans; j++){
        if(delete[j - a.start] != 1){
          for(s = 0; s < a.nbstates; s++){
            t0 = an->transitions[j];
            for(k = j+1;k < a.start + a.nbtrans; k++){
              if(delete[k-a.start] != 1){
                id = 0;
                Transition t = an->transitions[k];
                if(t0.fst == s && t.fst == s){
                  for(l = 0; l < t0.nbcond; l++){
                    for(m = 0; m < t.nbcond; m++){
                      if(t0.automCond[l]==t.automCond[m] && t0.stateCond[l]==t.stateCond[m]){
                        id += 1;
                      }
                    }
                  }
                  if(id == min(t0.nbcond,t.nbcond)){
                    if(t0.nbcond > t.nbcond){
                      an->transitions[j] = an->transitions[k];
                      t0 = t;
                      delete[k - a.start] = 1;
                      nbdelete += 1;
                    }
                    else{
                      delete[k - a.start] = 1;
                      nbdelete += 1;
                    }
                  }
                  if(id + 1 == t.nbcond && id + 1 == t0.nbcond){
                    for(l = 0; l < t.nbcond; l++){
                      if(t0.automCond[l] == t.automCond[l] && t0.stateCond[l] != t.stateCond[l]){
                        for(m = l; m < t0.nbcond - 1; m++){
                          t0.automCond[m] = t0.automCond[m+1];
                          t0.stateCond[m] = t0.stateCond[m+1];
                        }
                        t0.nbcond += -1;
                        delete[k - a.start] = 1;
                        nbdelete += 1;
                      }
                    }
                  }
                }
                an->transitions[j] = t0;
              }
            }
          }
        }
      }
      restart = nbdelete;
      l = 0;
      for(m = 0; m < a.nbtrans; m++){
        if(delete[m]){
          removeTransition(an, a.start + m - l);
          l += 1;
        }
      }
    }
  }
  // return an;
}

void simplifyAN(AN *an){

  int i;
  int s;
  int n;
  int nb;
  int restart = 1;
  int rmv = 1;
  Automata a;
  GlobalState g;
  while(restart){
    // printf("\nrestarting simplification\n\n");
    initAutomatas(an);
    simplifyTrans(an);
    // an = countTrans(an);
    countTrans(an);
    g = an->initial;
    restart = 0;
    i = 0;
    nb = an->nbauto;
    // printAN(an);
    for(n = 0; n < nb; n++){
      rmv = 1;
      a = an->automatas[i];
      for(s = 0; s < a.nbstates; s++){
        if(a.frequency[s] == 0){
          // printf("Processing automata %s from state %d : frequency %f\n",a.name,s,a.frequency[s]);
          if(rmv && g.states[i] == s){
            removeAutomata(an,i);
            g = an->initial;
            // printf("removed automata %s, case 1, state %d\n",a.name,s);
            // printf("i : %d, n : %d\n",i,n);
            restart = 1;
            rmv = 0;
          }
          // // simplification un peu violente
          // else {
          //   if(rmv && a.frequency[1-s] == 1){
          //     printf("removed automata %s, case 2, state %d\n",a.name,s);
          //     printf("i : %d, n : %d\n",i,n);
          //     g.states[i] = s;
          //     an->initial = g;
          //     an = removeAutomata(an,i);
          //     g = an->initial;
          //     restart = 1;
          //     rmv = 0;
          //   }
          // } //comment
        }
      }
      i += rmv;
    }
  }
  // return an;
}

int min(int a, int b){
  if(a < b){
    return a;
  } else {
    return b;
  }
}
