#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>


struct tablo {
	int * tab;
	int size;
};

void quicksort_aux(struct tablo * ta, int index_min, int index_max){

	int i,j;
	int v, t;

	if (index_max-index_min<2) return;

	//partition
	v = ta->tab[index_min];
	i=index_min;
	j=index_max;
	for(;;) {
		while (++i < index_max && ta->tab[i] < v);
		while (--j >= index_min && ta->tab[j] > v);
		if (i>=j) break;
		t = ta->tab[i]; ta->tab[i] = ta->tab[j]; ta->tab[j]=t;
	}
	t=ta->tab[i-1];ta->tab[i-1]=ta->tab[index_min]; ta->tab[index_min]=t;

	quicksort_aux(ta, index_min, i);
	quicksort_aux(ta, i, index_max );

}

void quicksort(struct tablo * ta, int index_min, int index_max) {

	int i,j;
	int v, t;

	if (index_max-index_min<2) return;

	//partition
	v = ta->tab[index_min];
	i=index_min;
	j=index_max;
	for(;;) {
		while (++i < index_max && ta->tab[i] < v);
		while (--j >= index_min && ta->tab[j] > v);
		if (i>=j) break;
		t = ta->tab[i]; ta->tab[i] = ta->tab[j]; ta->tab[j]=t;
	}
	t=ta->tab[i-1];ta->tab[i-1]=ta->tab[index_min]; ta->tab[index_min]=t;

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			quicksort_aux(ta, index_min, i);
		}
		#pragma omp section
		{
			quicksort_aux(ta, i, index_max );
		}
	}

}

void printArray(struct tablo * tmp) {
	printf("---- Array of size %i ---- \n", tmp->size);
	int size = tmp->size;
	int i;
	for (i = 0; i < size; ++i) {
		printf("%i ", tmp->tab[i]);
	}
	printf("\n");
}

struct tablo * allocateTablo(int size) {
	struct tablo * tmp = malloc(sizeof(struct tablo));
	tmp->size = size;
	tmp->tab = malloc(size*sizeof(int));
	return tmp;
}

void generateArray(struct tablo * s) {
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

void generateSortedArray(struct tablo *s, int size) {
	s->size=size;
	s->tab=malloc(size*sizeof(int));
	int i;
	for(i=0;i<size; i++) {
		s->tab[i]=i;
	}
}

int bracketRand(int a, int b){
  return rand()%(b-a) + a;
}

void generateRandomArray(struct tablo *s, int size) {
	s->size=size;
	s->tab=malloc(size*sizeof(int));
	int i;
	for(i=0;i<size; i++) {
		s->tab[i]=rand();
	}
}


void generateReverseSortedArray(struct tablo *s, int size) {
	s->size=size;
	s->tab=malloc(size*sizeof(int));
	int i;
	for(i=0;i<size; i++) {
		s->tab[i]=size-i;
	}
}


int main(int argc, char **argv) {
	struct tablo * tmp = malloc(sizeof(struct tablo));
	int threads =1;

	int arrSize=100000;
  if(argc >1){
      arrSize = atoi(argv[1]);
  }

	if (argc>2) {
		threads= atoi(argv[2]);
		#if defined(_OPENMP)
			omp_set_num_threads(threads);
		#endif
	}

	struct tablo * dryrun = malloc(sizeof(struct tablo));

	generateRandomArray(tmp, arrSize);

	generateArray(dryrun);
	quicksort(dryrun,0, dryrun->size);

	struct timeval temps_avant, temps_apres;
	gettimeofday (&temps_avant, NULL);

	quicksort(tmp,0, tmp->size);

	gettimeofday (&temps_apres, NULL);
	printf("%ld,", ((temps_apres.tv_sec - temps_avant.tv_sec) * 1000000 + temps_apres.tv_usec) - temps_avant.tv_usec);

}
