#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "premierjet.h"
#include "../smchr/smchr.h"

int main(void){

  clock_t temps;

  // AN an = getData("../models/mammalian.an");
  // AN an = getData("../models/MAPK.an");
  // AN an = getData("../models/ErBb2.an");
  // AN an = getData("../models/ErBb2_noinit.an");
  // AN an = getData("../models/egf_tnf1.an");
  // AN an = getData("../models/th_diff_binary.an");
  // AN an = getData("../models/th_diff_noinit_binary.an");
  AN an = getData("../models/phagelambda4binary.an");

  simplifyAN(&an);
  // printAN(an);
  // printAutomatas(an);
  Element *e = newElement(an.automatas[minfreq(an).automata],minfreq(an).state);
  AutoList *prev = newAutoList(e);
  AutoList *l1; // liste locale, représente un sous-bassin
  Basin *b = newBasin(prev); // automates déjà utilisés comme états initiaux
  BasinList *bl = newBasinList(b);
  int i;
  int restart;
  int nbproc = 2*an.nbauto; // nombre de recherche à effectuer. Un plus grand nombre trouve plus de bassins mais la recherche est plus longue.
                            // remarque : les premières recherches sont a priori plus fructueuses que les dernières.
  for(i = 0; i < nbproc; i++){
    // printf("\n Essai %d, Première Recherche\n",i);
    AutoList *l = findBasin(an);
    restart = l->result;
    while(restart){
      // printf("\nRelance\n");
      l1 = basinFromBasin(an, l, prev);
      if(l1->result){
        // commenter les trois lignes suivantes pour ne pas garder les sur-bassins
        // if(isIn(an,l,bl) == 0){
        //   pushBasin(bl,l);
        // }
        l = autoListCat(l,l1);
        l->result = 1;
      }
      else{
        restart = 0;
      }
    }
    an.automatas[minfreq(an).automata].frequency[minfreq(an).state] = 1;
    push(prev,an.automatas[minfreq(an).automata],minfreq(an).state);
    if(l->result){
      // printf("\nPlus grand bassin : longueur %d\n",length(l->first));
      // printList(l);
      if(isIn(an,l,bl) == 0){
        pushBasin(bl,l);
      }
    }
    else{
      // printf("\nPas de bassin pour cette recherche.\n");
    }
  }
  int total[an.nbauto];
  int nbtotal = 0;
  for(i = 0; i < an.nbauto; i++){
    total[i] = 0;
  }
  printf("\n Bassins trouvés : \n\n");
  bl->first = bl->first->next;
  b = bl->first;
  while(b!=NULL){
    printf("Bassin de longueur %d\n",length(b->autolist->first));
    total[length(b->autolist->first)] += 1;
    nbtotal += 1;
    printList(b->autolist);
    b = b->next;
  }

  // for(i = 0; i < an.nbauto; i++){
  //   printf("Bassins de longueur %d : %d\n",i,total[i]);
  // }
  printf("\nNombre de bassins : %d\n\n",nbtotal);

  int **m = fusionnable(an, bl);
  printf("\n\nMatrice de fusionnabilité\n\n");
  int lengthbl = basinLength(bl->first);
  int j;
  for(i = 0; i < lengthbl; i++){
    printf("\n");
    for(j = 0; j < lengthbl; j++){
      printf("%d ",m[i][j]);
    }
  }
  printf("\n");
  for(i = 0; i < lengthbl; i++){
    free(m[i]);
  }
  free(m);

  // MERGE

  BasinList *minimal = merge(an,bl);
  printf("\n MERGE \n Nombre de bassins minimaux : %d\n\n",basinLength(minimal->first));
  b = minimal->first;
  while(b!=NULL){
    printf("\nBassin de longueur %d\n\n",length(b->autolist->first));
    printList(b->autolist);
    b = b->next;
  }

  printf("\n");
  temps = clock();
  printf("Temps : %f seconde(s)\n",(double) temps/CLOCKS_PER_SEC);
  // printf("\n Nombre d'intersections : %d\n",countIntersections(an,bl));
  return 0;
}
