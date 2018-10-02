#include <stdlib.h>
#include <stdio.h>

#include "printAN.h"

void printAN(AN an){

  printAutomatas(an);
  printTransitions(an);
  printInitial(an);
  printFrequency(an);

}

void printAutomatas(AN an){

  printf("\nAUTOMATES : \n\n");
  int i;
  for (i = 0; i < an.nbauto;i++) {
    Automata a = an.automatas[i];
    printf("%d %s\n",a.number,a.name);
    int j;
    for (j = 0; j < a.nbstates;j++) {
      printf("%d\n", a.states[j]);
    }
  }

}

void printTransitions(AN an){

  printf("\nTRANSITIONS : \n\n");
  int i;
  for(i = 0; i < an.nbtrans; i++){
    Transition t = an.transitions[i];
    printf("Transition de l'automate : %s\n",an.automatas[t.automata].name);
    printf("de %d vers %d\n",t.fst,t.snd);
    int j;
    for(j = 0; j < t.nbcond; j++){
      printf("quand %s vaut %d\n",an.automatas[t.automCond[j]].name,t.stateCond[j]);
    }
  }

}

void printInitial(AN an){

  printf("\n");
  int i;
  for(i = 0; i < an.nbauto; i++){
    printf("Etat initial de l'automate %s : %d\n", an.automatas[i].name,an.initial.states[i]);
  }

}

void printFrequency(AN an){

  int i;
  for(i = 0; i < an.nbauto; i++){
    Automata a = an.automatas[i];
    printf("\nFrequence automate %s de 0 vers 1 : %f\n",a.name,a.frequency[0]);
    printf("Frequence automate %s de 1 vers 0 : %f\n",a.name,a.frequency[1]);
  }
  printf("\n");

}
