#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "transition.h"

void countTrans(AN *an){

  int local[an->nbauto];
  int i;
  int j;
  int k;
  int l;
  int m;
  int p;
  int q;
  int r;
  int s;
  int count;
  int n;
  int joint;
  double freq;
  Automata a;
  Transition t;
  Transition tab[an->nbauto];

  for(i = 0; i < an->nbauto; i++){
    // printf("Automate %s\n",an->automatas[i].name);
    a = an->automatas[i];
    for(s = 0; s < a.nbstates; s++){
      // printf("Etat %d\n",s);
      freq = 0;
      count = 0;
      if(a.start > -1 && a.nbtrans > 0){
        for(j = a.start; j < a.start+a.nbtrans; j++){
          t = an->transitions[j];
          if(t.fst == s){
            tab[count] = t;
            count += 1;
          }
        }
      }
      for(j = 0; j < count; j++){
        //terme d'ordre 1
        freq += 1/pow(2,tab[j].nbcond);
        // printf("ordre 1 : add %f\n",1/pow(2,tab[j].nbcond));
        for(k = 0; k < an->nbauto; k++){
          local[k] = -1;
        }
        for(k = 0; k < tab[j].nbcond; k++){
          local[tab[j].automCond[k]] = tab[j].stateCond[k];
        }
        for(k = j+1; k < count; k++){
          joint = 1;
          n = tab[j].nbcond + tab[k].nbcond;
          for(l = 0; l < tab[k].nbcond; l++){
            if(local[tab[k].automCond[l]] > -1){
              if(local[tab[k].automCond[l]] != tab[k].stateCond[l]){
                joint = 0;
              }
              else{
                n = n-1;
              }
            }
          }
          //terme d'ordre 2
          freq = freq - joint/pow(2,n);
          // printf("ordre 2 : substract %f\n",joint/pow(2,n));
          for(m = k+1; m < count; m++){
            n = tab[j].nbcond + tab[k].nbcond + tab[m].nbcond;
            for(l = 0; l < tab[j].nbcond; l++){
              for(q = 0; q < tab[k].nbcond; q++){ //comp 1/2
                if(tab[j].automCond[l] == tab[k].automCond[q]){
                  if(tab[j].stateCond[l] == tab[k].stateCond[q]){
                    n = n - 1;
                  }
                  else {
                      joint = 0;
                  }
                }
              }
              for(q = 0; q < tab[m].nbcond; q++){ // comp 1/3
                if(tab[j].automCond[l] == tab[m].automCond[q]){
                  if(tab[j].stateCond[l] == tab[m].stateCond[q]){
                    n = n - 1;
                  }
                  else {
                      joint = 0;
                  }
                }
              }
            }
            for(l = 0; l < tab[k].nbcond; l++){
              for(q = 0; q < tab[m].nbcond; q++){ // comp 2/3
                if(tab[k].automCond[l] == tab[m].automCond[q]){
                  if(tab[k].stateCond[l] == tab[m].stateCond[q]){
                    n = n - 1;
                  }
                  else {
                      joint = 0;
                  }
                }
              }
            }
            //terme d'ordre 3 : 1/2/3
            freq = freq + joint/pow(2,n);
            // printf("ordre 3 : add %f\n", joint/pow(2,n));
            for(p = m+1; p < count; p++){
              n = tab[j].nbcond + tab[k].nbcond + tab[m].nbcond + tab[p].nbcond;
              for(l = 0; l < tab[j].nbcond; l++){
                for(q = 0; q < tab[k].nbcond; q++){ //comp 1/2
                  if(tab[j].automCond[l] == tab[k].automCond[q]){
                    if(tab[j].stateCond[l] == tab[k].stateCond[q]){
                      n = n - 1;
                    }
                    else {
                        joint = 0;
                    }
                  }
                }
                for(q = 0; q < tab[m].nbcond; q++){ // comp 1/3
                  if(tab[j].automCond[l] == tab[m].automCond[q]){
                    if(tab[j].stateCond[l] == tab[m].stateCond[q]){
                      n = n - 1;
                    }
                    else {
                        joint = 0;
                    }
                  }
                }
                for(q = 0; q < tab[p].nbcond; q++){ // comp 1/4
                  if(tab[j].automCond[l] == tab[p].automCond[q]){
                    if(tab[j].stateCond[l] == tab[p].stateCond[q]){
                      n = n - 1;
                    }
                    else {
                        joint = 0;
                    }
                  }
                }
              }
              for(l = 0; l < tab[k].nbcond; l++){
                for(q = 0; q < tab[m].nbcond; q++){ //comp 2/3
                  if(tab[k].automCond[l] == tab[m].automCond[q]){
                    if(tab[k].stateCond[l] == tab[m].stateCond[q]){
                      n = n - 1;
                    }
                    else {
                        joint = 0;
                    }
                  }
                }
                for(q = 0; q < tab[p].nbcond; q++){ // comp 2/4
                  if(tab[k].automCond[l] == tab[p].automCond[q]){
                    if(tab[k].stateCond[l] == tab[p].stateCond[q]){
                      n = n - 1;
                    }
                    else {
                        joint = 0;
                    }
                  }
                }
              }
              for(l = 0; l < tab[m].nbcond; l++){
                for(q = 0; q < tab[p].nbcond; q++){ // comp 3/4
                  if(tab[m].automCond[l] == tab[p].automCond[q]){
                    if(tab[m].stateCond[l] == tab[p].stateCond[q]){
                      n = n - 1;
                    }
                    else {
                        joint = 0;
                    }
                  }
                }
              }
              //terme d'ordre 4 : 1/2/3/4
              freq = freq - joint/pow(2,n);
              // printf("ordre 4 : substract %f\n", joint/pow(2,n));
            }
          }
        }
      }
      if(freq < 0){
        freq = 0.01;
      }
      an->automatas[i].frequency[s] = freq;
      // printf("Frequence de %s dans l'etat %d : %f\n",an->automatas[i].name,s,freq);
    }
  }
  // return an;
}

void removeTransition(AN *an, int nb){

  int j;
  int anb = an->transitions[nb].automata;
  for(j = nb; j < an->nbtrans - 1; j++){
    an->transitions[j] = an->transitions[j+1];
  }
  an->nbtrans += -1;
  an->automatas[anb].nbtrans += -1;
  for(j = anb+1; j < an->nbauto; j++){
    an->automatas[anb].start += -1;
  }
  // return an;

}
