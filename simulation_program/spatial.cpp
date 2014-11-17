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
  
  if(argc != 7){
    std::cerr<<"spatial <outfile> <pattern> <pattern> <inter cell signal strength> <local field strength> <replicates>"<<std::endl;
    return EXIT_FAILURE;
  }

  std::string outfile(argv[1]);
  int pattern1 = atoi(argv[2]);
  int pattern2 = atoi(argv[3]);//1023 - pattern1;
  double sigstrength = atof(argv[4]);
  double lfstrength = atof(argv[5]);
  if(pattern1 > 1023){
    std::cerr<<"assume patterns from 0 to 1023 as models have length 10 with 2 states"<<std::endl;
    return EXIT_FAILURE;
  }
  
  int replicates = atoi(argv[6]); 
  while(replicates > 0){
    replicates--;
    std::ostringstream outname;
    outname << outfile  << "_" << replicates;
    /* create population */
    Pop* pop = (Pop*)malloc(sizeof(Pop));
    init_pop_growing(pop,10,10,10,2, sigstrength,lfstrength);
    init_models_growing(pop, pattern1, pattern2, rng);
    init_interactions(pop);
    int i,j,k;
    int generations = 30;
    while(generations > 0){
      generations--;
      /* set local fields */
      for(i = 0; i < pop->x; i++){
	for(j = 0; j< pop->y; j++){
	  if(pop->space[i][j] != NULL){
	    for(k = 0; k < pop->space[i][j]->length; k++){
	      pop->space[i][j]->localfield[k] = pop->localfieldstrength*(2.0*pop->space[i][j]->modelstring[k]-1.0);
	    }
	  }
	}
      }
      mh_pop(pop,rng,1000);
      cleanUp(pop,0,pattern1,pattern2);
      grow(pop,rng);
    }
    int p1cells = 0;
    int p2cells = 0;
    int** pM = patternMatrix(pop);
    for(i = 0; i < pop->x; i++){
      for(j = 1; j < pop->y; j++){
	if(pM[i][j] == pattern1)p1cells++;
	if(pM[i][j] == pattern2)p2cells++;
      }
    }
    std::cout<<p1cells<<std::endl<<p2cells<<std::endl;
    //for(i = 0; i < pop->x; i++)free(pM[i]);
    //free(pM);
    //del_pop(pop);
  }
  
  return EXIT_SUCCESS;
}
