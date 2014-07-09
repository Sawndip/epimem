#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
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
    num = (int)(num/states);
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
  if(argc != 4){
    std::cerr<<"./"<<std::string(argv[0])<<" <outputprefix> <part> <size>"<<std::endl;
    return EXIT_FAILURE;
  }
  int part = atoi(argv[2]);
  int size = atoi(argv[3]);
  int startat = part*size;
  int endat = (part + 1 ) * size - 1;
  
  Pottsmodel* model = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(model,10,2);
  unsigned int i,j;//,t
  int numberofpatterns = 2;
  int largest = (int)pow(model->states,model->length)/2 - 1;
  if(largest < endat)endat=largest;
  for(numberofpatterns = startat; numberofpatterns <= endat; numberofpatterns++){  
    //for(counter = 0; counter < 7; counter++){
    //numberofpatterns = numis[counter];
    int graph;
    for(graph = 0; graph < 1; graph++){//number of replicates
      std::string filename(argv[1]);
      std::ostringstream filencd;
      filencd<<filename<<"_"<<numberofpatterns<<"_"<<graph<<".celldivision";
      std::ostringstream filebasincd;
      filebasincd<<filename<<"_"<<numberofpatterns<<"_"<<graph<<".basinscd";
      std::ofstream cd;
      cd.open(filencd.str().c_str());
      std::ofstream basinscd;
      basinscd.open(filebasincd.str().c_str());
      /*reset*/
      for(i = 0; i < (unsigned int)model->length; i++){
	for(j = 0; j < (unsigned int)model->length; j++){
	  model->interactions[i][j] = 0.0;
	}
      }
      int p1 = 0;
      std::cout<<numberofpatterns<<" "<<graph<<std::endl;
      int2nary(numberofpatterns,model->modelstring,model->length,model->states);
    
    
      for(i = 0; i < (unsigned int)model->length; i++){
	//std::cout<<model->modelstring[i];
	if(model->modelstring[i] == 1)p1++;
	for(j = 0; j < (unsigned int)model->length; j++){
	  if(i != j){
	    if(model->modelstring[i] == model->modelstring[j]){model->interactions[i][j] += 1.0;}
	    else{ model->interactions[i][j] -= 1.0;}
	  }
	}
      }
      double erthreshold = 0.0;//log((double)model->length)/(double)model->length; 
      for(i = 0; i < (unsigned int)model->length; i++){
	for(j = i+1; j < (unsigned int)model->length; j++){
	  //double include = (double)rand()/(double)RAND_MAX;
	  //if(include >= erthreshold){
	  if((j-i) != 1){
	    model->interactions[i][j] = 0.0;
	    model->interactions[j][i] = 0.0;
	  }
	}
      }
   

  
      /****RANDOM INTERACTIONS****/
      /*for(i = 0; i < model->length; i++){
	for(j = 0; j < model->length; j++){
	if(i != j)model->interactions[i][j] = 2.0*((double)rand()/(double)RAND_MAX) - 1.0;
	}
	}
      */
      //std::cout<<model->length<<std::endl;
      for(i = 0; i < (unsigned int)model->length; i++){
	for(j = 0; j < (unsigned int)model->length; j++){
	  std::cout<<model->interactions[i][j]<<"\t";
	}
	std::cout<<std::endl;
      }
  



      /* do monte carlo simulation to find the distribution of the states */
  
      unsigned int generations = 30;
      int generationlength = 100;
      int b;
      std::vector<int> patterns;
      patterns.push_back(numberofpatterns);
      patterns.push_back((int)pow(model->states,model->length)-1-numberofpatterns);


  
      int* init = (int*)malloc(sizeof(int)*model->length);
      //std::cout<<"#CELLDIVISION"<<std::endl;
      for(b = 0; b < (int)patterns.size(); b++){
	int backtoorigin = 0;
	int converttodifferent = 0;
	int unstable = 0;
	int notzero = 0;
	int celldivstates = pow(2,notzero);
	int2nary(patterns[b],init,model->length,model->states);
	
	int r ;
	for(r = 0; r < 1000; r++){
	  /* set inital state*/
	  int2nary(patterns[b],model->modelstring,model->length,model->states);
	
	  /* set inital weak local field */
	  for(i = 0; i < (unsigned int)model->length; i++)model->localfield[i] = 0.1*(double)(2*model->modelstring[i] -1);
	
	  /* calculate possible cell divsiion states */
	  notzero = 0;
	  for(i = 0; i < (unsigned int)model->length; i++)if(init[i] > 0)notzero++;
	  celldivstates = pow(2,notzero);
	
	
	  for(i = 0; i < generations; i++){
	    int choose = rand() % celldivstates;
	    setState(model->modelstring,choose,notzero,model->modelstring,model->length);
	    int celldivstate = nary2int(model->modelstring,model->length,model->states);
	    int changes = mh(model,rng,generationlength);
	    if(changes > 0){
	      unstable++;
	    }else{
	      int res = nary2int(model->modelstring,model->length,model->states);
	      basinscd<<patterns[b]<<"\t"<<celldivstate<<"\t"<<res<<"\t"<<i<<"\t"<<r<<std::endl;
	      if(patterns[b] == res){
		backtoorigin++;
	      }else{
		converttodifferent++;
	      }
	    }
	  
	    /* recalculate all stuff */
	    for(j = 0; j < (unsigned int)model->length; j++)model->localfield[j] = 0.1*(double)(2*model->modelstring[j] -1);
	    notzero = 0;
	    for(j = 0; j < (unsigned int)model->length; j++)if(model->modelstring[j] > 0)notzero++;
	    celldivstates = pow(2,notzero);


	  }
	}
	cd<<patterns[b]<<"\t"<<backtoorigin<<"\t"<<converttodifferent<<"\t"<<unstable<<std::endl; 
      }
      cd.close();
      basinscd.close();
      /* gzip files */
      std::string gzip("gzip ");
      std::string cmd = gzip;
      cmd.append(filencd.str());
      filencd<<".gz";
      remove(filencd.str().c_str());
      system(cmd.c_str());
      cmd = gzip;
      cmd.append(filebasincd.str());
      filebasincd<<".gz";
      remove(filebasincd.str().c_str());
      system(cmd.c_str());
    }
  }
  del(model);
  return EXIT_SUCCESS;
}
