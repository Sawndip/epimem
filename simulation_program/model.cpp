#include "model.hpp"
#include <cmath>
#include <algorithm>    // std::move_backward
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

void initialize(Pottsmodel* p, int l, int s){
  p->length = l;
  p->states = s;
  p->pos = new std::vector<int>();
  p->pos->reserve(p->length);
  /* allocate memory for interactions */
  unsigned int i,j;
  p->interactions = (double**)malloc(sizeof(double*)*p->length);
  for(i = 0; i < (unsigned int)p->length; i++){
    p->pos->push_back((int)i);
    p->interactions[i] = (double*)malloc(sizeof(double)*p->length);
    for(j = 0; j < (unsigned int)p->length; j++){
      p->interactions[i][j] = 0.0;
    }
  }
  
  /* allocate memory for model string and init randomly */
  p->modelstring = (int*)malloc(sizeof(int)*p->length);
  for(i = 0; i < (unsigned int)p->length; i++)p->modelstring[i] = 0;
  
  p->localfield = (double*)malloc(sizeof(double)*p->length);
  for(i = 0; i <(unsigned int) p->length; i++)p->localfield[i] = 0.0;
    
  p->cellfield = (double*)malloc(sizeof(double)*p->length);
  for(i = 0; i <(unsigned int) p->length; i++)p->cellfield[i] = 0.0;
  
  
}

int update(Pottsmodel* p){
  unsigned int i,j;
  int changes = 0;
  int* newstate = (int*)malloc(sizeof(int)*p->length);
  for(i = 0; i < (unsigned int)p->length; i++){
    double input = 0.0;
    newstate[i] = p->modelstring[i];
    for(j = 0; j < (unsigned int)p->length; j++)if(i != j && p->modelstring[i] == p->modelstring[i])input += p->interactions[i][j];
    if(input > 0.0){
      if(p->modelstring[i] == 0)changes++;
      newstate[i] = 1;
    }else{ 
      if(p->modelstring[i] == 1)changes++;
      newstate[i] = 0;
    }
  }
  free(p->modelstring);
  p->modelstring = newstate;
  return changes;
}

int mhStep(Pottsmodel* p, std::default_random_engine rng){
  int changes = 0;
  unsigned int i,j,k;
  std::shuffle (p->pos->begin(), p->pos->end(), rng);
  for(i = 0; i < (unsigned int)p->length; i++){
    int site = p->pos->at(i);
    int newstate = rand() % p->states;
    int oldstate = p->modelstring[site];
    while(newstate == p->modelstring[site])newstate = rand() % p->states;
    double e_bef = 0.0;
    for(j = 0; j < (unsigned int)p->length; j++){
      for(k = 0; k < (unsigned int)p->length; k++){
	if(p->modelstring[j] == p->modelstring[k])
	  e_bef += p->interactions[j][k];
      }
      if(p->modelstring[j] == 1)e_bef += p->localfield[j];
      else e_bef -= p->localfield[j];
      e_bef += p->cellfield[j]*(2.0*(double)p->modelstring[j]-1);
    }
    p->modelstring[site] = newstate;
    double e_aft = 0.0;
    for(j = 0; j < (unsigned int)p->length; j++){
      for(k = 0; k < (unsigned int)p->length; k++){
	if(p->modelstring[j] == p->modelstring[k])
	  e_aft += p->interactions[j][k];
      }
      if(p->modelstring[j] == 1)e_aft += p->localfield[j];
      else e_aft -= p->localfield[j];
      e_aft += p->cellfield[j]*(2.0*(double)p->modelstring[j]-1);
    }
    double pr = exp(-e_bef+e_aft) > 1.0 ? 1.0 : exp(-e_bef+e_aft); 
    double accept = (double)rand() / (double)RAND_MAX;
    if(pr < accept)p->modelstring[site] = oldstate;
    else changes++;
  }
  
  return changes;
}

int mh(Pottsmodel* p,std::default_random_engine rng, int upperbound){
  int counter = 0;
  int changes = p->length;
  while( changes > 0 && counter < upperbound){
    changes = mhStep(p,rng);
    counter++;
  }
  return changes;
}

void del(Pottsmodel* p){
  unsigned int i,j;
  delete p->pos;
  for(i = 0; i < (unsigned int)p->length; i++)free(p->interactions[i]);
  free(p->interactions);
  free(p->modelstring);
  free(p->localfield);
  free(p->cellfield);
}


int nary2int(Pottsmodel* p){
  int i;
  int num = 0;
  
  for(i = 0; i < p->length; i++){
    num += p->modelstring[i]*(int)pow(p->states,i);
  } 
  
  return num;
}
