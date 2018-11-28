#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "getData.h"

AN getData(char *filepath){

  FILE *file = NULL;
  file = fopen(filepath,"r");
  if(file == NULL){
    printf("Unable to access file ");
    // goto done;
  }

  AN an;
  GlobalState g;
  int k;
  for(k = 0; k < MAX_AUTOMATAS; k++){
    g.states[k] = -1;
  }

  char line[MAX_LENGTH];
  int nbauto = 0;
  an.nbtrans = 0;

  int err;
  regex_t preg;

  int match;
  //longueurs pour les formules regex
  int len1 = 30 + 12*(MAX_STATES-1);
  int len2 = 76 + 37*(MAX_CONDITIONS-1);
  // int len3 = 45 + 36*(MAX_AUTOMATAS-1);
  // const char *str_regex_auto = "(\"[-_[:alnum:]]+\") \\[([0-9]) ?(, ?([0-9]))*\\]";
  // const char *str_regex_trans = "(\"[-_[:alnum:]]+\") ([0-9]) ?-> ?([0-9])( when (\"[-_[:alnum:]]+\") ?= ?([0-9])( and (\"[-_[:alnum:]]+\") ?= ?([0-9]))*)?";

  char str_regex_auto_p[len1];
  char str_int1[len1];
  strcpy(str_regex_auto_p,"(\"[-_[:alnum:]]+\") \\[([0-9]) ?");
  // printf("longueur base : %d\n", strlen(str_regex_auto_p));
  // printf("longueur ajout : %d\n", strlen("(, ?([0-9]))"));
  for(k = 1; k< MAX_STATES;k++){
    // strcpy(str_regex_auto_p,strcat(str_regex_auto_p,"(, ?([0-9]))?"));
    strcpy(str_int1, strcat(str_regex_auto_p,"(, ?([0-9]))?"));
    strcpy(str_regex_auto_p,str_int1);
  }
  char *str_regex_auto = NULL;
  str_regex_auto = malloc(sizeof(char)*(len1+6));
  strcpy(str_regex_auto,strcat(str_regex_auto_p,"\\]"));


  char str_regex_trans_p[len2];
  char str_int2[len2];
  strcpy(str_regex_trans_p,"(\"[-_[:alnum:]]+\") ([0-9]) ?-> ?([0-9])( when (\"[-_[:alnum:]]+\") ?= ?([0-9])");
  // printf("longueur base : %d\n", strlen("(\"[-_[:alnum:]]+\") ([0-9]) ?-> ?([0-9])( when (\"[-_[:alnum:]]+\") ?= ?([0-9])"));
  // printf("longueur ajout : %d\n", strlen("( and (\"[-_[:alnum:]]+\") ?= ?([0-9]))"));
  for(k = 1; k<MAX_CONDITIONS; k++){
    // strcpy(str_regex_trans_p,strcat(str_regex_trans_p,"( and (\"[-_[:alnum:]]+\") ?= ?([0-9]))?"));
    strcpy(str_int2,strcat(str_regex_trans_p,"( and (\"[-_[:alnum:]]+\") ?= ?([0-9]))?"));
    strcpy(str_regex_trans_p,str_int2);
  }
  char *str_regex_trans = NULL;
  str_regex_trans = malloc(sizeof(char)*(len2+9));
  strcpy(str_regex_trans,strcat(str_regex_trans_p,")?"));

  // char str_regex_init_p[len3];
  // strcpy(str_regex_init_p,"initial_context (\"[-_[:alnum:]]\") ?= ?([0-9])");
  // // printf("longueur base : %d\n", strlen("initial_context \"[-_[:alnum:]]\" ?= ?([0-9])"));
  // // printf("longueur ajout : %d\n", strlen("(, ?\"[-_[:alnum:]] ?= ?([0-9]))?"));
  // for(k = 1; k<MAX_AUTOMATAS; k++){
  //   strcpy(str_regex_init_p,strcat(str_regex_init_p,"(, ?(\"[-_[:alnum:]]\") ?= ?([0-9]))?"));
  // }
  char *str_regex_init = NULL;
  // str_regex_init = malloc(sizeof(char)*len3);
  // strcpy(str_regex_init,str_regex_init_p);

  // printf("test_auto : %s\n", str_regex_auto);
  // printf("test_trans : %s\n",str_regex_trans);
  // printf("test_init : %s\n",str_regex_init);
  // printf("\n");

  while(fgets(line,MAX_LENGTH,file) != NULL){

    err = regcomp(&preg,str_regex_auto,REG_EXTENDED);

    size_t nmatch = 0;
    regmatch_t *pmatch = NULL;
    nmatch = 3 + 2*MAX_STATES;
    pmatch = malloc (sizeof (*pmatch) * nmatch);

    match = regexec(&preg,line,nmatch,pmatch,0);
    regfree(&preg);

    if(match == 0){

      // printf("match for line \n");
      // printf("%s",line);

      Automata a;
      a.number = nbauto;
      a.nbstates = 0;

      char *site = NULL;
      int start = pmatch[1].rm_so;
      int end = pmatch[1].rm_eo;
      size_t size = end - start;

      site = malloc (sizeof (*site) * (size + 1));
      strncpy(site, &line[start], size);
      site[size] = '\0';
      // printf("%s\n", site);
      strncpy(a.name,site,size+1);
      free (site);

      int i;
      for (i = 0; i < MAX_STATES; i++) {
        // char *site = NULL;
        int start = pmatch[2+2*i].rm_so;
        int end = pmatch[2+2*i].rm_eo;
        size_t size = end - start;

        // site = malloc (sizeof (*site) * (size + 1));
        if(size){
          //  strncpy(site, &line[start], size);
          //  a.states[i] = atoi(site);
          //  printf("%s\n", site);
          //  free (site);
          a.states[i] = atoi(&line[pmatch[2+2*i].rm_so]);
          a.nbstates += 1;
        }
      }

      an.automatas[nbauto] = a;
      nbauto += 1;
      an.nbauto = nbauto;
      free(pmatch);
    }

    else {
      free(pmatch);
      err = regcomp(&preg,str_regex_trans,REG_EXTENDED);
      size_t nmatch = 0;
      regmatch_t *pmatch = NULL;
      nmatch = 6 + 3*MAX_CONDITIONS;
      pmatch = malloc (sizeof (*pmatch) * nmatch);
      match = regexec(&preg,line,nmatch,pmatch,0);
      regfree(&preg);
      if(match == 0){
        // printf("match for line :\n");
        // printf("%s\n",line);

        Transition t;

        //nom de l'automate
        char *site = NULL;
        int start = pmatch[1].rm_so;
        int end = pmatch[1].rm_eo;
        size_t size = end - start;
        site = malloc (sizeof (*site) * (size + 1));
        strncpy(site,&line[start],size);
        site[size] = '\0';
        // printf("%s\n",site );
        int i;
        for(i = 0; i < nbauto; i++){
          Automata a = an.automatas[i];
          if(strncmp(site,a.name,size) == 0){
            // printf("%s est l'automate numero %d\n",site,a.number);
            t.automata = a.number;
          }
        }
        free(site);
        //transition depuis
        t.fst = atoi(&line[pmatch[2].rm_so]);
        //transition vers
        t.snd = atoi(&line[pmatch[3].rm_so]);
        //conditions
        t.nbcond = 0;
        for(i = 0; i < MAX_CONDITIONS; i++){
          start = pmatch[5+3*i].rm_so;
          end = pmatch[5+3*i].rm_eo;
          size = end - start;
          if(size){
            t.stateCond[i] = atoi(&line[pmatch[6+3*i].rm_so]);
            site = malloc(sizeof(*site)*(size+1));
            strncpy(site,&line[start],size);
            site[size] = '\0';
            int j;
            for(j = 0; j < nbauto; j++){
              Automata a = an.automatas[j];
              if(strncmp(site,a.name,size) == 0){
                // printf("%s est l'automate numero %d\n",site,a.number);
                t.automCond[i] = a.number;
              }
            }
            free(site);
            t.nbcond += 1;
          }
        }
        an.transitions[an.nbtrans] = t;
        an.nbtrans += 1;
        free(pmatch);
      }
      else{
        free(pmatch);
        int len3 = 46 + 37*(an.nbauto-1);

        char str_regex_init_p[len3];
        char str_int3[len3];
        strcpy(str_regex_init_p,"initial_context (\"[-_[:alnum:]]+\") ?= ?([0-9])");
        // printf("longueur base : %d\n", strlen("initial_context \"[-_[:alnum:]]\" ?= ?([0-9])"));
        // printf("longueur ajout : %d\n", strlen("(, ?\"[-_[:alnum:]] ?= ?([0-9]))?"));
        for(k = 1; k<an.nbauto; k++){
          // strcpy(str_regex_init_p,strcat(str_regex_init_p,"(, ?(\"[-_[:alnum:]]+\") ?= ?([0-9]))?"));
          strcpy(str_int3,strcat(str_regex_init_p,"(, ?(\"[-_[:alnum:]]+\") ?= ?([0-9]))?"));
          strcpy(str_regex_init_p,str_int3);
        }
        str_regex_init = malloc(sizeof(char)*len3);
        strcpy(str_regex_init,str_regex_init_p);

        // printf("test_init : %s\n",str_regex_init);

        err = regcomp(&preg,str_regex_init,REG_EXTENDED);
        size_t nmatch = 0;
        regmatch_t *pmatch = NULL;
        nmatch = 2 + 3*an.nbauto;
        pmatch = malloc (sizeof (*pmatch) * nmatch);
        match = regexec(&preg,line,nmatch,pmatch,0);
        regfree(&preg);
        if(match == 0){

          // GlobalState g;
          g.nbauto = an.nbauto;
          // for(k = 0; k < an.nbauto; k++){
          //   g.states[k] = -1;
          // }

          for(k = 0; k < an.nbauto; k++){
            int start = pmatch[1+3*k].rm_so;
            int end = pmatch[1+3*k].rm_eo;
            size_t size = end - start;
            if(size){
              char *site = NULL;
              site = malloc(sizeof(*site)*(size+1));
              strncpy(site,&line[start],size);
              site[size] = '\0';
              int j;
              for(j = 0; j < nbauto; j++){
                Automata a = an.automatas[j];
                // printf("Comparing %s to %s, value %d",site,a.name,atoi(&line[pmatch[2+3*k].rm_so]));
                if(strncmp(site,a.name,size)==0){
                  g.states[a.number] = atoi(&line[pmatch[2+3*k].rm_so]);
                }
              }
              free(site);
            }
          }
        }
        free(pmatch);
        free(str_regex_init);
      }
    }
    // free(pmatch);
  }
  g.nbauto = an.nbauto;
  an.initial = g;
  fclose(file);
  free(str_regex_trans);
  free(str_regex_auto);
  initAutomatas(&an);
  return an;
}
