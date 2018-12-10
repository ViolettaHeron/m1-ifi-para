#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int bracketRand(int a, int b){
  return rand()%(b-a) + a;
}

void createRandomArray(int size, int* arr){

  for(int i=0;i<size;i++){
    arr[i] = bracketRand(0, 50);
  }
}

void arrayToPower(int* arr, int power, int size, int nbThreads){
  #if defined(_OPENMP)
    omp_set_num_threads(nbThreads);
  #endif
  #pragma omp parallel for
  for(int i=0;i<size;i++){
    arr[i] = pow(arr[i], power);
  }
}

void printArray(int* array, int size){
    for(int i=0;i<size;i++){
      printf("%d, ", array[i]);
    }
    printf("\n");
}

void main(int argc, char* argv[]){
  srand(time(NULL));
  int arrSize = 500;
  int nbOfThreads = 2;

  if(argc >= 2){
      arrSize = atoi(argv[1]);
  }
  if(argc >= 3) {
      nbOfThreads = atoi(argv[2]);
  }
  int* randomArray = (int*)malloc(arrSize * sizeof(int));

  createRandomArray(arrSize, randomArray);
  // printArray(randomArray, arrSize);

  arrayToPower(randomArray, 3, arrSize, nbOfThreads);
  // printArray(randomArray, arrSize);

  free(randomArray);
}
