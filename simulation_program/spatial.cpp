#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <time.h>       /* time */
#include "model.hpp"
#include "population.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cstring>
#include <map>
#include <vector>
#include <cmath>
#include <random>
#include <utility>
#include <chrono>

int main(int argc, char* argv[]){
  srand(time(NULL));
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng(seed); /*default uniform random number generator */
  
  if(argc != 5){
    std::cerr<<"spatial <outfile> <pattern> <pattern> <replicates>"<<std::endl;
    return EXIT_FAILURE;
  }

  std::string outfile(argv[1]);
  int pattern1 = atoi(argv[2]);
  int pattern2 = atoi(argv[3]);//1023 - pattern1;
  if(pattern1 > 1023){
    std::cerr<<"assume patterns from 0 to 1023 as models have length 10 with 2 states"<<std::endl;
    return EXIT_FAILURE;
  }
  
  int replicates = atoi(argv[4]); 
  while(replicates > 0){
    replicates--;
    std::ostringstream outname;
    outname << outfile  << "_" << replicates;
    /* create population */
    Pop* pop = (Pop*)malloc(sizeof(Pop));
    init_pop(pop,10,10,10,2);
    init_models(pop, pattern1, pattern2, rng);
    init_interactions(pop);
    int** pM = patternMatrix(pop);
    int i,j,k;
    for(i = 0; i < pop->x; i++){
      std::cerr<<pM[i][0]<<" ";
      for(j = 1; j < pop->y; j++){
	std::cerr<<" "<<pM[i][j];
      }
      std::cerr<<std::endl;
    } 
    std::cerr<<std::endl<<std::endl;
    int generations = 10;
    while(generations > 0){
      generations--;
      /* set local fields */
      for(i = 0; i < pop->x; i++){
	for(j = 0; j< pop->y; j++){
	  if(pop->space[i][j] != NULL){
	    for(k = 0; k < pop->space[i][j]->length; k++){
	      pop->space[i][j]->localfield[k] = 0.5*(2.0*pop->space[i][j]->modelstring[k]-1.0);
	    }
	  }
	}
      }
      mh_pop(pop,rng,1000);
      for(i = 0; i < pop->x; i++)free(pM[i]);
      free(pM);
      pM = patternMatrix(pop);
      for(i = 0; i < pop->x; i++){
	std::cerr<<pM[i][0]<<" ";
	for(j = 1; j < pop->y; j++){
	  std::cerr<<" "<<pM[i][j];
	}
	std::cerr<<std::endl;
      } 
      std::cerr<<std::endl<<std::endl;
      cleanUp(pop,0,pattern1,pattern2);
      for(i = 0; i < pop->x; i++)free(pM[i]);
      free(pM);
      pM = patternMatrix(pop);
      for(i = 0; i < pop->x; i++){
	std::cerr<<pM[i][0]<<" ";
	for(j = 1; j < pop->y; j++){
	  std::cerr<<" "<<pM[i][j];
	}
	std::cerr<<std::endl;
      } 
      std::cerr<<std::endl<<std::endl;
      grow(pop,rng);
      for(i = 0; i < pop->x; i++)free(pM[i]);
      free(pM);
      pM = patternMatrix(pop);
      for(i = 0; i < pop->x; i++){
	std::cerr<<pM[i][0]<<" ";
	for(j = 1; j < pop->y; j++){
	  std::cerr<<" "<<pM[i][j];
	}
	std::cerr<<std::endl;
      } 
      std::cerr<<std::endl<<std::endl;
    }
    del_pop(pop);
  }
  
  return EXIT_SUCCESS;
}
