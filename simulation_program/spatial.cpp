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
  
  int p,q;
  int** popMaPat1 = (int**)malloc(10*sizeof(int*));
  int** popMaPat2 = (int**)malloc(10*sizeof(int*));
  std::ostringstream pat1Outname;
  pat1Outname<<outfile<<".pattern1";
  std::ostringstream pat2Outname;
  pat2Outname<<outfile<<".pattern2";
  std::ofstream pat1Out;
  std::ofstream pat2Out;
  pat1Out.open(pat1Outname.str().c_str());
  pat2Out.open(pat2Outname.str().c_str());
  
  

  for(p = 0; p < 10; p++){
    popMaPat1[p] = (int*)malloc(10*sizeof(int));
    popMaPat2[p] = (int*)malloc(10*sizeof(int));
    for(q = 0; q < 10; q++){
      popMaPat1[p][q] = 0;
      popMaPat2[p][q] = 0;
    }
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
	if(pM[i][j] == pattern1){
	  p1cells++;
	  popMaPat1[i][j]++;
	}
	if(pM[i][j] == pattern2){
	  p2cells++;
	  popMaPat2[i][j]++;
	}
      }
    }
    //std::cout<<"1\t"<<pattern1<<"\t"<<p1cells<<std::endl;
    //std::cout<<"2\t"<<pattern2<<"\t"<<p2cells<<std::endl;
    //for(i = 0; i < pop->x; i++)free(pM[i]);
    //free(pM);
    //del_pop(pop);
  }

  for(p = 0; p < 10; p++){
    pat1Out<<popMaPat1[p][0];
    pat2Out<<popMaPat2[p][0];
    for(q = 1; q < 10; q++){
      pat1Out<<"\t"<<popMaPat1[p][q];
      pat2Out<<"\t"<<popMaPat2[p][q];

    }
    pat1Out<<std::endl;
    pat2Out<<std::endl;
  }
  pat1Out.close();
  pat2Out.close();

  return EXIT_SUCCESS;
}
