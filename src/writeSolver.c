#include <stdlib.h>
#include <stdio.h>

#include "writeSolver.h"

void writeSolver(AN an, char *filepath){

  FILE *file = NULL;
  file = fopen(filepath,"w+");
  if(file == NULL){
    printf("Unable to access file ");
    // goto done;
  }
  else{
    printf("Opened file %s\n",filepath);

    char *line;
    int i;
    int j;
    int k;

    for(i = 0; i < an.nbauto; i++){
      fprintf(file,"type f%d(num",an.automatas[i].number);
      for(j = 1;j < an.nbauto; j++){
        fprintf(file,",num");
      }
      fprintf(file,");\n");
    }

    fprintf(file,"type attract(num");
    for(j = 1; j < an.nbauto; j++){
      fprintf(file,",num");
    }
    fprintf(file,");\n\n");

    fprintf(file,"states(x) <=> x=0 \\/ x=1;\n\n");

    Automata a;
    Transition t;
    int clause;
    int done;
    int s;
    for(i = 0; i < an.nbauto; i++){
      a = an.automatas[i];
      for(s = 0; s < a.nbstates; s++){
        clause = 0;
        done = 0;
        for(j = 0; j < an.nbtrans; j++){
          t = an.transitions[j];
          if(1-done && a.number == t.automata && t.fst == s){
            if(t.nbcond == 0){
              done = 1;
              if(a.number == 0){
                fprintf(file,"f0(%d",s);
                for(k = 1; k < an.nbauto; k++){
                  fprintf(file,",x%d",k);
                }
                fprintf(file,") <=> true;\n");
                fprintf(file,"not f0(%d",s);
                for(k = 1; k < an.nbauto; k++){
                  fprintf(file,",x%d",k);
                }
                fprintf(file,") <=> false;\n\n");
              }
              else{
                fprintf(file,"f%d(x0",a.number);
                for(k = 1; k < an.nbauto; k++){
                  if(k == a.number){
                    fprintf(file,",%d",s);
                  }
                  else{
                    fprintf(file,",x%d",k);
                  }
                }
                fprintf(file,") <=> true;\n");
                fprintf(file,"not f%d(x0",a.number);
                for(k = 1; k < an.nbauto; k++){
                  if(k == a.number){
                    fprintf(file,",%d",s);
                  }
                  else{
                    fprintf(file,",x%d",k);
                  }
                }
                fprintf(file,") <=> false;\n\n");
              }
            }
            else{
              fprintf(file,"type clause%d%d%d(num",a.number,s,clause);
              for(k = 1; k < an.nbauto; k++){
                fprintf(file,",num");
              }
              fprintf(file,");\n");

              if(a.number == 0){
                fprintf(file,"clause%d%d%d(%d",a.number,s,clause,s);
              }
              else{
                fprintf(file,"clause%d%d%d(x0",a.number,s,clause);
              }
              for(k = 1; k < an.nbauto; k++){
                if(k == a.number){
                  fprintf(file,",%d",s);
                }
                else{
                  fprintf(file,",x%d",k);
                }
              }
              fprintf(file,") <=> ");
              fprintf(file,"x%d=%d",t.automCond[0],t.stateCond[0]);
              for(k = 1; k < t.nbcond; k++){
                fprintf(file," /\\ x%d=%d",t.automCond[k],t.stateCond[k]);
              }
              fprintf(file,";\n");

              if(a.number == 0){
                fprintf(file,"not clause%d%d%d(%d",a.number,s,clause,s);
              }
              else{
                fprintf(file,"not clause%d%d%d(x0",a.number,s,clause);
              }
              for(k = 1; k < an.nbauto; k++){
                if(k == a.number){
                  fprintf(file,",%d",s);
                }
                else{
                  fprintf(file,",x%d",k);
                }
              }
              fprintf(file,") <=> ");
              fprintf(file,"x%d!=%d",t.automCond[0],t.stateCond[0]);
              for(k = 1; k < t.nbcond; k++){
                fprintf(file," \\/ x%d!=%d",t.automCond[k],t.stateCond[k]);
              }
              fprintf(file,";\n");
              clause += 1;
            }
          }
        }

        if(1-done && clause == 0){
          fprintf(file,"\nf%d(",a.number);
          if(a.number == 0){
            fprintf(file,"%d",s);
          }
          else{
            fprintf(file,"x0");
          }
          for(k = 1; k < an.nbauto; k++){
            if(k == a.number){
              fprintf(file,",%d",s);
            }
            else{
              fprintf(file,",x%d",k);
            }
          }
          fprintf(file,") <=> false;\n");

          fprintf(file,"not f%d(",a.number);
          if(a.number == 0){
            fprintf(file,"%d",s);
          }
          else{
            fprintf(file,"x0");
          }
          for(k = 1; k < an.nbauto; k++){
            if(k == a.number){
              fprintf(file,",%d",s);
            }
            else{
              fprintf(file,",x%d",k);
            }
          }
          fprintf(file,") <=> true;\n\n");
        }

        if(1-done && clause > 0){
          fprintf(file,"\nf%d(",a.number);
          if(a.number == 0){
            fprintf(file,"%d",s);
          }
          else{
            fprintf(file,"x0");
          }
          for(k = 1; k < an.nbauto; k++){
            if(k == a.number){
              fprintf(file,",%d",s);
            }
            else{
              fprintf(file,",x%d",k);
            }
          }
          fprintf(file,") <=> clause%d%d%d(",a.number,s,0);
          if(a.number == 0){
            fprintf(file,"%d",s);
          }
          else{
            fprintf(file,"x0");
          }
          for(k = 1; k < an.nbauto; k++){
            if(k == a.number){
              fprintf(file,",%d",s);
            }
            else{
              fprintf(file,",x%d",k);
            }
          }
          fprintf(file,")");
          for(k = 1; k < clause; k++){
            fprintf(file, " \\/ clause%d%d%d(",a.number,s,k);
            if(a.number == 0){
              fprintf(file,"%d",s);
            }
            else{
              fprintf(file,"x0");
            }
            for(k = 1; k < an.nbauto; k++){
              if(k == a.number){
                fprintf(file,",%d",s);
              }
              else{
                fprintf(file,",x%d",k);
              }
            }
            fprintf(file,")");
          }
          fprintf(file,";\n");

          fprintf(file,"not f%d(",a.number);
          if(a.number == 0){
            fprintf(file,"%d",s);
          }
          else{
            fprintf(file,"x0");
          }
          for(k = 1; k < an.nbauto; k++){
            if(k == a.number){
              fprintf(file,",%d",s);
            }
            else{
              fprintf(file,",x%d",k);
            }
          }
          fprintf(file,") <=> not clause%d%d%d(",a.number,s,0);
          if(a.number == 0){
            fprintf(file,"%d",s);
          }
          else{
            fprintf(file,"x0");
          }
          for(k = 1; k < an.nbauto; k++){
            if(k == a.number){
              fprintf(file,",%d",s);
            }
            else{
              fprintf(file,",x%d",k);
            }
          }
          fprintf(file,")");
          for(k = 1; k < clause; k++){
            fprintf(file, " /\\ not clause%d%d%d(",a.number,s,k);
            if(a.number == 0){
              fprintf(file,"%d",s);
            }
            else{
              fprintf(file,"x0");
            }
            for(k = 1; k < an.nbauto; k++){
              if(k == a.number){
                fprintf(file,",%d",s);
              }
              else{
                fprintf(file,",x%d",k);
              }
            }
            fprintf(file,")");
          }
          fprintf(file,";\n\n");
        }
      }
    }

    GlobalState g = an.initial;
    fprintf(file,"attract(");
    if(g.states[0] > -1){
      fprintf(file,"%d",g.states[0]);
    }
    else{
      fprintf(file,"x0");
    }
    for(k = 1; k < an.nbauto; k++){
      if(g.states[k] > -1){
        fprintf(file,",%d",g.states[k]);
      }
      else{
        fprintf(file,",x%d",k);
      }
    }
    fprintf(file,") <=> false;\n\n");

    for(i = 0; i < an.nbauto; i++){
      fprintf(file,"attract(x0");
      for(j = 1; j < an.nbauto; j++){
        fprintf(file,",x%d",j);
      }
      fprintf(file,") ==> notx%d := 1-x%d | not f%d(x0",i,i,i);
      for(j = 1; j < an.nbauto; j++){
        fprintf(file,",x%d",j);
      }
      fprintf(file,") \\/ attract(");
      if(i == 0){
        fprintf(file,"notx0");
      }
      else{
        fprintf(file,"x0");
      }
      for(j = 1; j < an.nbauto; j++){
        if(i==j){
          fprintf(file,",notx%d",j);
        }
        else{
          fprintf(file,",x%d",j);
        }
      }
      fprintf(file,");\n");
    }

    fclose(file);
  }

}
