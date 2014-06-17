#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <deque>
#include <time.h>       /* time */
#include "model.hpp"
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

int noncelldivisiondiffs(int* parent, int* child, int length){
  int i, nonallowedtransitions = 0;
  for(i = 0; i < length;i++)if(parent[i] == 0 && child[i] == 1)nonallowedtransitions++;
  return nonallowedtransitions;
}

void int2nary(int num, int* bin, int length, int states){
  int i;
  for(i = 0; i < length; i++){
    bin[i] = num % states;
    num = num/states;
  }
  
}

int nary2int(int* bin, int length, int states){
  int i;
  int num = 0;
  
  for(i = 0; i < length; i++){
    num += bin[i]*(int)pow(states,i);
  } 
  
  return num;
}



void setState(int* target,int mask,int masklength,int* init,int length){
  int* maskArray = (int*)malloc(sizeof(int)*masklength);
  int2nary(mask,maskArray,masklength,2);
  int c = 0;
  int i;
  for(i = 0; i < length; i++){
    if(init[i] > 0){
      if(maskArray[c] == 0)target[i] = 0;
      else target[i] = init[i];
      c++;
    }else target[i] = init[i];
  }
  free(maskArray);
}


int main(int argc, char* argv[]){
  srand (time(NULL));
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rng(seed); /*default uniform random number generator */

  Pottsmodel* model = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(model,10,3);
  
  unsigned int i,j;
  //int2binary(pow(2,(int)(model->length/2))-1,model->modelstring,model->length);
  //for(i = 0; i < model->length; i++){
  //  for(j = 0; j < model->length; j++){
  //    if(i != j)model->interactions[i][j] = (double)(2 * model->modelstring[i] - 1)*(2 * model->modelstring[j] - 1);
  //  }
  //}
  /****RANDOM INTERACTIONS****/
  
  for(i = 0; i < model->length; i++){
    for(j = 0; j < model->length; j++){
      if(i != j)model->interactions[i][j] = 2.0*((double)rand()/(double)RAND_MAX) - 1.0;
    }
  }
  
  std::cout<<model->length<<std::endl;
  for(i = 0; i < model->length; i++){
    for(j = 0; j < model->length; j++){
      std::cout<<model->interactions[i][j]<<"\t";
    }
    std::cout<<std::endl;
  }
  



  /* do monte carlo simulation to find the distribution of the states */
  //TODO
  std::map<int,int> states;
  std::map<int,int> statesunf;
  
  int samples = 1000;
  int upbou = 100000;
  int max = pow(model->states,model->length);
  for(i = 0; i < samples; i++){
    for(j = 0; j < model->length; j++){
      model->modelstring[j] = rand() % model->states;
      int changes = mh(model,rng,upbou);
      int curstate = nary2int(model->modelstring,model->length,model->states);
      if(changes > 0){
	if(statesunf.find(curstate) == statesunf.end())statesunf[curstate] = 1;
	else statesunf[curstate]++;
      }else{
	if(states.find(curstate) == states.end())states[curstate] = 1;
	else states[curstate]++;
      }
    }
  } 
  
  std::cout<<"STATES WITHOUT CHANGES"<<std::endl;
  std::cout<<"END STATE\tFREQUENCY"<<std::endl;
  std::map<int,int>::iterator it, it2;
  
  for(it = states.begin(); it != states.end(); it++){
    int2nary(it->first,model->modelstring,model->length,model->states); 
    std::cout<<it->first<<"\t";
    for(i = 0; i < model->length; i++)std::cout<<model->modelstring[i];
    std::cout<<"\t"<<it->second<<std::endl;
  }
  std::cout<<"STATES WITH CHANGES"<<std::endl;
  std::cout<<"END STATE\tFREQUENCY"<<std::endl;
  for(it = statesunf.begin(); it != statesunf.end(); it++){
    int2nary(it->first,model->modelstring,model->length,model->states); 
    std::cout<<it->first<<"\t";
    for(i = 0; i < model->length; i++)std::cout<<model->modelstring[i];
    std::cout<<"\t"<<it->second<<std::endl;
  }
  
  int* init = (int*)malloc(sizeof(int)*model->length);
  std::cout<<"#CELL DIVISION BASIN"<<std::endl;
  std::cout<<"INIT\tINIT\tCELL DIVISON PATTERN\tRECOVERED\tDIFFERENTBASIN\tUNSTABLE\n";
  std::cerr<<states.size()<<std::endl;
  for(it = states.begin(); it != states.end(); it++){
    int backtoorigin = 0;
    int converttodifferent = 0;
    int unstable = 0;
    int2nary(it->first,init,model->length,model->states);
    int notzero = 0;
    for(i = 0; i < model->length; i++)if(init[i] > 0)notzero++;
    int celldivstates = pow(2,notzero);
    std::map<int,int> fixesfound;
    int totalunst = 0;
    for(i = 0; i < celldivstates; i++){
      std::map<int,int> fix;
      int unst  = 0;
      for(j = 0; j < samples; j++){
	setState(model->modelstring,i,notzero,init,model->length);
	int changes = mh(model,rng,upbou);
	if(changes > 0){
	  unst++;
	  totalunst++;
	}else{
	  int res = nary2int(model->modelstring,model->length,model->states);
	  if(fix.find(res) != fix.end())fix[res]++;
	  else fix[res] = 1;
	  if(fixesfound.find(res) != fixesfound.end())fixesfound[res]++;
	  else fixesfound[res] = 1;
	}
      }
      if(unstable > 0 || fix.size() > 1)unstable++;
      else{
	if(fix.begin()->first == it->first)backtoorigin++;
	else converttodifferent++;
      }
    }
    int total = backtoorigin + converttodifferent + unstable;
    std::cout<<it->first<<"\t";
    std::cerr<<it->first<<"\t"<<totalunst;
    for(it2 = fixesfound.begin(); it2 != fixesfound.end(); it2++)std::cerr<<"\t"<<it2->first<<" ("<<it2->second<<")";
    std::cerr<<std::endl;
    for(i = 0; i < model->length; i++)std::cout<<init[i];
    std::cout<<"\t"<<total<<"\t"<<backtoorigin<<" ("<<(double)backtoorigin/(double)total<<")\t";
    std::cout<<converttodifferent<<" ("<<(double)converttodifferent/(double)total<<")\t";
    std::cout<<unstable<<" ("<<(double)unstable/(double)total<<")"<<std::endl;
  }
  
  del(model);
  

  return EXIT_SUCCESS;
}
