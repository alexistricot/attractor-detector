#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "frequency.h"

minFrequency minfreq(AN an){

  minFrequency m;
  m.frequency = 1;
  int i;
  int s;
  for(i = 0; i < an.nbauto; i++){
    Automata a = an.automatas[i];
    for(s = 0; s < a.nbstates; s++){
      if(a.frequency[s] < m.frequency && a.frequency[s] > 0){
        m.automata = i;
        m.state = s;
        m.frequency = a.frequency[s];
      }
    }
  }
  return m;
}

double freqLocal(AN an, int automata, int state, int *cond){

  int local[an.nbauto];
  int j;
  int k;
  int l;
  int m;
  int n;
  int p;
  int q;
  int r;
  int s;
  double freq = 0; // fréquence à calculer
  int count = 0; //nombre de transitions dans tab
  int joint;
  Automata a = an.automatas[automata];
  Transition tab[a.nbtrans]; //contient les transitions qui nous intéressent
  Transition t;
  if(a.start > -1 && a.nbtrans > 0){ //on remplit tab
    for(j = a.start; j < a.start+a.nbtrans; j++){
      t = an.transitions[j];
      if(t.fst == state){
        tab[count] = t;
        count += 1;
      }
    }
  }
  int delete[count]; //transitions à enlever de tab après les deux boucles qui suivent
  for(j = 0; j < count; j++){ // initialisation
    delete[j] = 0;
  }
  for(j = 0; j < count; j++){ // on modifie la transition tab[j]
    t = tab[j];
    m = 0; // compteur a priori égal à k, sauf si on enlève une condition
    p = t.nbcond;
    for(k = 0; k < p; k++){
      if(cond[t.automCond[m]] > -1 && cond[t.automCond[m]] == t.stateCond[m]){ //condition satisfaite : on enlève la condition
        t.nbcond += -1;
        for(l = m; l < t.nbcond; l++){
          t.automCond[l] = t.automCond[l+1];
          t.stateCond[l] = t.stateCond[l+1];
        }
      }
      else{
        if(cond[t.automCond[m]] > -1 && cond[t.automCond[m]] != t.stateCond[m]){ //contradiction : on enlève la transition
          delete[j] = 1;
          // printf("contradiction ! transition %d\n",j);
        }
        m += 1;
      }
    }
    tab[j] = t;
  }
  m = 0; // compteur égal à j, sauf si on supprime une transition
  p = count;
  for(j = 0; j < p; j++){ //suppression des transitions contradictoires avec cond
    if(delete[j]){
      count += -1;
      // printf("suppression de %d\n",j);
      for(l = m; l < count; l++){
        tab[l] = tab[l+1];
      }
    }
    else{
      m += 1;
    }
  }
  // calcul de la fréquence ; approximmation à l'ordre 4
  for(j = 0; j < count; j++){
    //terme d'ordre 1
    freq += 1/pow(2,tab[j].nbcond);
    // printf("ordre 1 : add %f\n",1/pow(2,tab[j].nbcond));
    for(k = 0; k < an.nbauto; k++){
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
  return freq;
}
