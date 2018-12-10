#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

struct tablo {
  int * tab;
  int size;
};

// neutral element of our operation
int neutral(){
  return 0;
}

//our operation
int op(int a, int b){
  return a+b;
}


void printArray(struct tablo * tmp) {
  printf("---- Array of size %i ---- \n[ ", tmp->size);
  int size = tmp->size;
  int i;
  for (i = 0; i < size; ++i) {
    printf("%d ", tmp->tab[i]);
  }
  printf("] \n");
}

struct tablo * allocateTablo(int size) {
  struct tablo * tmp = malloc(sizeof(struct tablo));
  tmp->size = size;
  tmp->tab = malloc(size*sizeof(int));
  return tmp;
}

void generateRandomArray(struct tablo *s, int size) {
  s->size=size;
  s->tab=malloc(size*sizeof(int));
  int i;
  for(i=0;i<size; i++) {
    s->tab[i]=rand() % 150;
  }
}

void montee(struct tablo * source, struct tablo * destination) {
  //TODO : remplissage du tableau destination de taille 2*n en
  // copiant les données du tableau source dans destination,
  // à la bonne position
  // on suppose que le malloc de destination a été fait avant
  #pragma omp parallel for
  for(int i = source->size; i< destination->size; i++){
    destination->tab[i] = source->tab[i - source->size];
  }

  // TODO: Boucle de calcul pour la montée dans l'arbre/tableau
  int m = (int)log2(source->size);
  for(int l = m-1; l >= 0; l--){
    #pragma omp parallel for
    for(int j = pow(2,l);j <= (int)pow(2, l+1) -1; j++){
      destination->tab[j] = op(destination->tab[2*j] ,destination->tab[2*j+1]);
    }
  }
}

void descente(struct tablo * a, struct tablo * b) {
  // implementation de la phase de descente
  b->tab[1] = neutral();
  int m = (int)log2(a->size);
  for(int l=1; l <= m-1; l++){
    #pragma omp parallel for
    for(int j=pow(2, l); j <= (int)pow(2,l+1) -1; j++){
      if(j%2 == 0){
        b->tab[j] = b->tab[j/2];
      }
      else{
        b->tab[j] = op(a->tab[j-1], b->tab[j/2]);
      }
    }
  }
}

void final(struct tablo * a, struct tablo *b) {
  // phase finale
  int m = (int)log2(a->size);
  #pragma omp parallel for
  for(int j=pow(2, m-1); j <= (int)pow(2,m) -1; j++){
    b->tab[j] = op(b->tab[j], a->tab[j]);
  }
}

void generateArray(struct tablo * s) {
  //construction d'un tableau pour tester
  s->size=8;
  s->tab=malloc(s->size*sizeof(int));
  s->tab[0]=3;
  s->tab[1]=1;
  s->tab[2]=7;
  s->tab[3]=0;
  s->tab[4]=4;
  s->tab[5]=1;
  s->tab[6]=6;
  s->tab[7]=3;
}

int main(int argc, char **argv) {
  struct tablo source;
  int size = (int)pow(2,3);
  if(argc > 1){
    size = (int)pow(2, atoi(argv[1]));
  }

  generateRandomArray(&source, size);


  struct tablo * a = allocateTablo(source.size*2);
  montee(&source, a);
  // printArray(a);

  struct tablo * b = allocateTablo(source.size*2);
  descente(a, b);
  // printArray(b);

  final(a,b);
  // printArray(b);
}
