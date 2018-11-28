#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "merge.h"


Basin *intersect(AN an, Basin *b1, Basin *b2){

  AutoList *a1 = malloc(sizeof(*a1));
  a1 = b1->autolist;
  AutoList *a2 = malloc(sizeof(*a2));
  a2 = b2->autolist;
  renumber(an,a1);
  renumber(an,a2);
  int local[an.nbauto]; //local[i] valeur de l'automate i dans a1
  int i;
  for(i = 0; i < an.nbauto; i++){
    local[i] = -1;
  }
  int disjoint = 0;
  Element *e1;
  AutoList *result = NULL;
  Element *e = a1->first;
  while(e!=NULL){
    if(result == NULL){
      e1 = newElement(e->automata,e->state);
      result = newAutoList(e1);
    }
    else{
      push(result, e->automata, e->state);
    }
    local[e->automata.number] = e->state;
    e = e->next;
  }

  e = a2->first;
  while(e!=NULL){
    if(local[e->automata.number]==-1){
      push(result, e->automata, e->state);
    }
    else{
      if(local[e->automata.number] != e->state){
        disjoint = 1;
      }
    }
    e = e->next;
  }
  // free(a1);
  // free(a2);
  if(disjoint){
    return NULL;
  }
  else{
    Basin *b = newBasin(result);
    return b;
  }
}

int **fusionnable(AN an, BasinList *bl){

  // printf("Entering fusionnables \n");
  int i;
  int j;
  int n = basinLength(bl->first);
  printf("n fusionnable = %d\n",n);
  int **m;
  m = malloc(n*sizeof(int*));
  for(i = 0; i < n; i++){
    m[i] = malloc(n*sizeof(int));
  }
  // printf("m allocated. \n");
  // printf("initializing m...");
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      m[i][j] = -1;
      m[j][i] = -1;
    }
  }
  Basin *b = bl->first;
  Basin *b1;
  Basin *inter;
  for(i = 0; i < n; i++){
    b1 = bl->first;
    // printf("Bassin %d : \n",i);
    // printList(b->autolist);
    for(j = 0; j < i; j++){
      // printf("fusionnable i %d j %d\n",i,j);
      // printf("Comparé à %d : \n",j);
      // printList(b1->autolist);
      inter = intersect(an, b, b1);
      if(inter == NULL){
        // printf("value of %d %d : 1\n",i,j);
        m[i][j] = 0;
        m[j][i] = 0;
      }
      else{
        // printf("value of %d %d : 0\n",i,j);
        m[i][j] = 1;
        m[j][i] = 1;
      }
      if(b1!=NULL){
        b1 = b1->next;
      }
    }
    m[i][i] = 1;
    if(b!=NULL){
      b = b->next;
    }
  }
  // if(b!=NULL){
  //   freeBasin(b);
  // }
  // if(b1!=NULL){
  //   freeBasin(b1);
  // }
  if(inter!=NULL){
    freeBasin(inter);
  }
  return m;
}

void bronKerbosch(int *r, int *p, int *x, int *np, int *nx, int n, int **m, CliqueList *cl){

  if(np[0] == 0 && nx[0] == 0){
    printf("adding clique : \n");
    printClique(r,n);
    pushClique(cl,r);
  }
  else{
    // printf("new BK step : ");
    // printf("np = %d, nx = %d\n", *np, *nx);
    // printf("r : ");
    // printClique(r, n);
    // printf("p : ");
    // printClique(p, n);
    // printf("x : ");
    // printClique(x, n);
    int v;
    for(v = 0; v < n; v++){
      if(p[v] == 1){
        int *vp = malloc(n*sizeof(int));
        vp = voisins(p,m,n,v,np);
        int *vx = malloc(n*sizeof(int));
        vx = voisins(x,m,n,v,nx);
        int *vr = malloc(n*sizeof(int));
        vr = rRec(n,v,r);
        // printf("Après voisins : np = %d, nx = %d\n", *np, *nx);
        // printf("vr : ");
        // printClique(vr, n);
        // printf("vp : ");
        // printClique(vp, n);
        // printf("vx : ");
        // printClique(vx, n);
        bronKerbosch(vr,vp,vx,np,nx,n,m,cl);
        // free(vr);
        // free(vp);
        // free(vx);
        p[v] = 0;
        x[v] = 1;
      }
    }
    // free(r);
  }
  // free(p);
  // free(x);
}

int *voisins(int *px, int **m, int n, int v, int *npx){
  int *t = malloc(n*sizeof(int));
  *npx = 0;
  int i;
  for(i = 0; i < n; i++){
    if(i != v){
      t[i] = px[i]*m[v][i];
      *npx += px[i]*m[v][i];
    }
    else{
      t[v] = 0;
    }
  }
  t[v] = 0;
  return t;
}

int *rRec(int n, int v, int *r){
  int *res = malloc(n*sizeof(int));
  // int res[n];
  int j;
  for(j = 0; j < n; j++){
    res[j] = r[j];
  }
  res[v] = 1;
  return res;
}

BasinList *merge(AN an, BasinList *bl){

  printf("\nStarting merge...\n");
  int n = basinLength(bl->first);
  printf("n : %d\n",n);
  printf("getting fus. matrix...\n");
  int **m = fusionnable(an, bl);
  printf("got fus. matrix.\n\n");
  int *r = malloc(n*sizeof(int));
  int *p = malloc(n*sizeof(int));
  int *x = malloc(n*sizeof(int));
  int j;
  for(j = 0; j < n; j++){
    r[j] = 0;
    p[j] = 1;
    x[j] = 0;
  }
  int *np = malloc(sizeof(int));
  *np = n;
  int *nx = malloc(sizeof(int));
  *nx = 0;
  CliqueList *cl = newCliqueList(NULL);
  printf("Starting Bron-Kerbosch algorithm...\n");
  bronKerbosch(r,p,x,np,nx,n,m,cl);
  printf("Exiting Bron-Kerbosch algorithm...\n\n");
  // free(m);
  Clique *c = cl->first;
  BasinList *result = newBasinList(NULL);
  int start;
  while(c != NULL){
    start = 1;
    Basin *b;
    for(j = 0; j < n; j++){
      // printf("c[%d] : %d", j, c->vertices[j]);
      if(c->vertices[j]){
        Basin *temp = getBasin(bl->first,j);
        if(start){
          b = temp;
          start = 0;
        }
        else{
          b = intersect(an,b,temp);
        }
      }
    }
    // printf("adding basin\n");
    // printList(b->autolist);
    pushBasin(result, b->autolist);
    c = c->next;
  }
  // freeCliqueList(cl->first);
  free(np);
  free(nx);
  return result;
}
