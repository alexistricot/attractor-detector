#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "attractor.h"

AutoList *findBasin(AN an){

  int cond[an.nbauto]; //tableau des conditions, une ajoutée à chaque tour
  int candidats[an.nbauto]; //tableau des candidats, -1 si pas candidat, état sinon (0 ou 1)
  int nbcandidats;
  int nbcond;
  minFrequency m = minfreq(an);
  Automata a = an.automatas[m.automata]; //automate en cours de traitement
  // Transition t;

  // printf("\nDépart : %s = %d\n",an.automatas[m.automata].name,m.state);


  int i;
  int j;
  double minimum;
  int candidatminimum;
  double somme;
  int failed; //recommencer quand on ne trouve rien
  if(an.nbauto > 15){
    failed = 5;
  }
  else{
    failed = 1;
  }
  int restart = 1;

  Element *e;
  AutoList *l;
  Element *e2;
  AutoList *lcond;

  while(failed){
    //initialisation des tableaux
    for(i = 0; i < an.nbauto; i++){
        cond[i] = -1;
        candidats[i] = -1;
    }
    cond[m.automata] = m.state;

    e = newElement(a,m.state);
    l = newAutoList(e);
    e2 = newElement(a,m.state);
    lcond = newAutoList(e2);
    lcond->result = 0;

    restart = 1;
    nbcandidats = 0;
    nbcond = 1;
    for(i = a.start; i < a.start + a.nbtrans; i++){ //initialisation des candidats
      Transition t = an.transitions[i];
      if(t.fst == m.state){
        for(j = 0; j < t.nbcond; j++){
          if(cond[t.automCond[j]] == -1 && candidats[t.automCond[j]] == -1){
            candidats[t.automCond[j]] = 1-t.stateCond[j];
            nbcandidats += 1;
          }
        }
      }
    }
    while(restart){ //recherche du bassin
      // printf("Relance interne \n");
      if(freqLocal(an,l->first->automata.number,l->first->state,cond) == 0){
        //alors on passe à l'automate de l suivant
        // printList(l);
        // printf("freqLocal nul, %s %d\n",l->first->automata.name,l->first->state);
        if(l->first->next == NULL){
          // printf("dernier élément de l, stop\n");
          restart = 0;
          nbcandidats = 0;
        }
        else{
          // printList(l);
          // printList(lcond);
          // printf("passage à l'élément suivant : ");
          l->first = l->first->next;
          // printf("%s %d\n",l->first->automata.name,l->first->state);
          // printList(l);
          // printList(lcond);
          for(i = 0; i < an.nbauto; i++){
              candidats[i] = -1;
          }
          a = l->first->automata;
          nbcandidats = 0;
          for(i = a.start; i < a.start + a.nbtrans; i++){ //initialisation des candidats du nouvel état "first"
            Transition t = an.transitions[i];
            if(t.fst == l->first->state){
              for(j = 0; j < t.nbcond; j++){
                if(cond[t.automCond[j]] == -1){
                  candidats[t.automCond[j]] = 1-t.stateCond[j];
                  nbcandidats += 1;
                }
              }
            }
          }
          // printf("nbcandidats : %d\n",nbcandidats);
        }
      }
      minimum = 1000; //à méditer, max_int ?
      if(nbcandidats == 0){
        restart = 0;
      }
      else{
        for(i = 0; i < an.nbauto; i++){ //recherche du candidat pour lequel la somme est minimale
          somme = 0;
          if(candidats[i] > -1){ //c'est un candidat
            cond[i] = candidats[i];
            for(j = 0; j < an.nbauto; j++){ //calcul de la somme
              if(cond[j] > -1){
                somme += freqLocal(an,j,cond[j],cond);
              }
            }
            if(somme < minimum){
              minimum = somme;
              candidatminimum = i;
            }
            cond[i] = -1;
          }
        }
        //on ajoute le candidat à cond et on recommence
        // printf("minimum : %f\n",minimum);
        cond[candidatminimum] = candidats[candidatminimum];
        nbcond += 1;
        push(l,an.automatas[candidatminimum],candidats[candidatminimum]);
        push(lcond,an.automatas[candidatminimum],candidats[candidatminimum]);
        candidats[candidatminimum] = -1;
        nbcandidats += -1;
        // printf("ajout cond %d %s %d\n",candidatminimum,an.automatas[candidatminimum].name,cond[candidatminimum]);
        // printf("nbcond : %d\n",nbcond);
        // printf("nbcandidats : %d\n",nbcandidats);
        // printList(l);
        if(minimum == 0){ //somme nulle, cond est un attracteur
          restart = 0;
        }
      }
    }
    if(minimum == 0){ //on a trouvé un bassin
      failed = 0;
      // printf("Bassin trouvé, %d conditions\n",nbcond);
      lcond->result = 1;
      for(i = 0; i < an.nbauto; i++){
        if(cond[i] > -1){
          // printf("\t %s = %d\n",an.automatas[i].name,cond[i]);
        }
      }
    }
    else{
      if(failed > 0){
        // printf("Plus de dépendance après %d conditions\n",nbcond);
        lcond->result = 0;
        for(i = 0; i < an.nbauto; i++){
          if(cond[i] > -1){
            // printf("\t %s = %d\n",an.automatas[i].name,cond[i]);
          }
        }
        failed = failed - 1;
        an.automatas[m.automata].frequency[m.state] = 1;
        m = minfreq(an);
        a = an.automatas[m.automata];
        // if(m.frequency == 0 || m.frequency == 1){
        //   failed = 0;
        // }
      }
    }
  }
  // freeList(l->first);
  // free(l);
  return lcond;
}

AutoList *basinFromBasin(AN an0, AutoList *l, AutoList *prev){

  AN an = an0;  // on crée une copie du réseau
                // on va simplifier le nouveau réseau pour qu'il corresponde au bassin codé par l
  Element *e = l->first;
  int delete[an.nbauto];
  int i;
  int n = an.nbauto;
  // printf("prev\n");
  // printList(prev);
  // printf("l\n");
  // printList(l);
  for(i = 0; i < an.nbauto; i++){
    delete[i] = 0;
  }
  while(e != NULL){
    for(i=0; i < an.nbauto; i++){
      if(strcmp(e->automata.name, an.automatas[i].name)==0){
        e->automata.number = i;
      }
    }
    an.initial.states[e->automata.number] = e->state;
    delete[e->automata.number] = 1;
    e = e->next;
  }
  for(i = n-1; i > -1; i--){
    if(delete[i]){
      // printf("removing automata %s\n",an.automatas[i].name);
      removeAutomata(&an,i);
    }
  }
  simplifyAN(&an);
  // printAutomatas(an);
  e = prev->first;
  while(e != NULL){
    for(i=0; i < an.nbauto; i++){
      if(strcmp(e->automata.name, an.automatas[i].name)==0){
        e->automata.number = i;
      }
    }
    an.automatas[e->automata.number].frequency[e->state] = 1;
    e = e->next;
  }
  return findBasin(an);

}
