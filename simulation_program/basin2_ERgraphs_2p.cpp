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
    std::cerr<<"./basins <outputprefix> <part> <size>"<<std::endl;
    return EXIT_FAILURE;
  }
  int part = atoi(argv[2]);
  int size = atoi(argv[3]);
  int startat1 = 0,startat2 = 1;
  
  Pottsmodel* model = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(model,10,2);
  /* calculate start and end for calculation */
  int largest = (int)pow(model->states,model->length)/2 - 1;
  int patternsinround = largest + 1;
  int calcbef = part * size + 1;
  while(calcbef > patternsinround){
    calcbef -= patternsinround;
    patternsinround--;
    startat1++;
    startat2++;
  }
  startat2 += calcbef;
  
  int endat1 = 0, endat2 = 1;
  calcbef = (part+1) * size;
  patternsinround = largest + 1;
  while(calcbef > patternsinround){
    calcbef -= patternsinround;
    patternsinround--;
    endat1++;
    endat2++;
  }
  endat2 += calcbef;
  //int temp[2] = {(int)pow(2,0)+(int)pow(2,1)+(int)pow(2,2),(int)pow(2,9)+(int)pow(2,8)+(int)pow(2,7)};
  unsigned int i,j,t;
  int rememp1 = startat1;
  int rememp2 = startat2;
  int totalmemories = 1;//(int)pow(model->states,model->length)/2;
  //int numis[7] = {0,511,255,256,257,63,65};
  //int counter;
  //for(rememp1 = 0; rememp1 <= largest; rememp1++){  
  //for(rememp2 = rememp1+1; rememp2 <= largest; rememp2++){  
  while(rememp1 <= endat1 || rememp2 <= endat2){
    //for(counter = 0; counter < 7; counter++){
    //numberofpatterns = numis[counter];
    std::string filename(argv[1]);
    //std::ostringstream fileno;
    //fileno<<filename<<"_"<<rememp1<<"_"<<rememp2<<".original";
    std::ostringstream filencd;
    filencd<<filename<<"_part"<<part<<"size"<<size<<"_"<<rememp1<<"_"<<rememp2<<".celldivision";
    //std::ostringstream filebasin;
    //filebasin<<filename<<"_"<<rememp1<<"_"<<rememp2<<".basins";;
    std::ostringstream filebasincd;
    filebasincd<<filename<<"_part"<<part<<"size"<<size<<"_"<<rememp1<<"_"<<rememp2<<".basinscd";
    //std::ofstream org;
    //org.open(fileno.str().c_str());
    std::ofstream cd;
    cd.open(filencd.str().c_str());
    //std::ofstream basins;
    //basins.open(filebasin.str().c_str());
    std::ofstream basinscd;
    basinscd.open(filebasincd.str().c_str());
    /*reset*/
    for(i = 0; i < model->length; i++){
      for(j = 0; j < model->length; j++){
	model->interactions[i][j] = 0.0;
      }
    }
    int maxpattern = (int)pow(model->states,model->length)/2 - 1;
    /*pattern learning*/
    int p1 = 0;
    std::cout<<"pattern to remember:"<<rememp1<<" "<<rememp2<<std::endl;
    int2nary(rememp1,model->modelstring,model->length,model->states);
    for(i = 0; i < model->length; i++){
      //std::cout<<model->modelstring[i];
      if(model->modelstring[i] == 1)p1++;
      for(j = 0; j < model->length; j++){
	if(i != j)
	  if(model->modelstring[i] == model->modelstring[j])model->interactions[i][j] += 0.5;
	  else model->interactions[i][j] -= 0.5;
      }
    }
    int2nary(rememp2,model->modelstring,model->length,model->states);
    for(i = 0; i < model->length; i++){
      //std::cout<<model->modelstring[i];
      if(model->modelstring[i] == 1)p1++;
      for(j = 0; j < model->length; j++){
	
	if(i != j)
	  if(model->modelstring[i] == model->modelstring[j])model->interactions[i][j] += 0.5;
	  else model->interactions[i][j] -= 0.5;
      }
    }
    /*erdoes renyi graph*/
    double erthreshold = log((double)model->length)/(double)model->length;
    for(i = 0; i < model->length; i++){
      //std::cout<<model->modelstring[i];
      if(model->modelstring[i] == 1)p1++;
      for(j = 0; j < model->length; j++){
	double include = rand()/RAND_MAX;
	if(i != j && include > erthreshold)model->interactions[i][j] = 0.0;
      }
    }
    /* calculate localfield */
    //int p2 = model->length - p1;
    //int p1cdstates = pow(2,p1);
    //int p2cdstates = pow(2,p2);
    //double p1prob = (double)(p1cdstates - p2cdstates)/(double)(p1cdstates+p2cdstates);
    //double p2prob = (double)(p2cdstates - p1cdstates)/(double)(p1cdstates+p2cdstates);
    //model->localfield = 0.0;//((double)p1*p1prob+(double)p2*p2prob)/(double)(p1+p2);

    //std::cout<<std::endl;
    
  
    /****RANDOM INTERACTIONS****/
    /*for(i = 0; i < model->length; i++){
      for(j = 0; j < model->length; j++){
      if(i != j)model->interactions[i][j] = 2.0*((double)rand()/(double)RAND_MAX) - 1.0;
      }
      }
    */
    std::cout<<model->length<<std::endl;
    for(i = 0; i < model->length; i++){
      for(j = 0; j < model->length; j++){
	std::cout<<model->interactions[i][j]<<"\t";
      }
      std::cout<<std::endl;
    }
  



    /* do monte carlo simulation to find the distribution of the states */
    //TODO
    //std::map<int,int> states;
  
    int samples = 1000;
    int upbou = 1000000;
    int max = pow(model->states,model->length);
    int b;
    std::vector<int> patterns;
    patterns.push_back(rememp1);
    patterns.push_back((int)pow(model->states,model->length)-1-rememp1);
    patterns.push_back(rememp2);
    patterns.push_back((int)pow(model->states,model->length)-1-rememp2);
    
    //for(b = 0; b < patterns.size(); b++){
    //	int unstable = 0;
    //	int diff = 0;
    //	int same = 0;
    //	for(i = 0; i < samples; i++){
    //	  int2nary(patterns[b],model->modelstring,model->length,model->states);
    //	  int changes = mh(model,rng,upbou);
    //	  int curstate = nary2int(model->modelstring,model->length,model->states);
    //	  if(changes > 0){
    //	    unstable++;
    //	  }else{
    //	    //if(states.find(curstate) == states.end())states[curstate] = 1;
    //	    //else states[curstate]++;
    //	    if(curstate == patterns[b]){
    //	      same++;
    //	    }else{
    //	      diff++;
    //	    }
    //	  }    
    //	}
    //	org<<patterns[b]<<"\t"<<same<<"\t"<<diff<<"\t"<<unstable<<std::endl; 
    //
    /* basins of original model */
    //for(b = 0; b < (int)pow(model->states,model->length); b++){
    //	for( i = 0; i < samples; i++){
    //	  int2nary(b,model->modelstring,model->length,model->states);
    //	  int changes = mh(model,rng,upbou);
    //	  int curstate = nary2int(model->modelstring,model->length,model->states);
    //	  if(changes == 0)basins<<b<<"\t"<<curstate<<std::endl;
    //	}
    //}




  
    int* init = (int*)malloc(sizeof(int)*model->length);
    //std::cout<<"#CELLDIVISION"<<std::endl;
    for(b = 0; b < patterns.size(); b++){
      int backtoorigin = 0;
      int converttodifferent = 0;
      int unstable = 0;
      int2nary(patterns[b],init,model->length,model->states);
      /* set weak local field */
      for(i = 0; i < model->length; i++)model->localfield[i] = 0.1*(double)(2*init[i] -1);
      int notzero = 0;
      for(i = 0; i < model->length; i++)if(init[i] > 0)notzero++;
      int celldivstates = pow(2,notzero);
      for(i = 0; i < celldivstates; i++){
	for(j = 0; j < samples; j++){
	  setState(model->modelstring,i,notzero,init,model->length);
	  int celldivstate = nary2int(model->modelstring,model->length,model->states);
	  int changes = mh(model,rng,upbou);
	  if(changes > 0){
	    unstable++;
	  }else{
	    int res = nary2int(model->modelstring,model->length,model->states);
	    basinscd<<patterns[b]<<"\t"<<celldivstate<<"\t"<<res<<std::endl;
	    if(patterns[b] == res)backtoorigin++;
	    else converttodifferent++;
	  }
	}
      }
      cd<<patterns[b]<<"\t"<<backtoorigin<<"\t"<<converttodifferent<<"\t"<<unstable<<std::endl; 
    }
    //org.close();
    cd.close();
    //basins.close();
    basinscd.close();
    /* gzip files */
    std::string gzip("gzip ");
    std::string cmd = gzip;
    //cmd.append(fileno.str());
    //fileno<<".gz";
    //remove(fileno.str().c_str());
    //system(cmd.c_str()); 
    //cmd = gzip;
    cmd.append(filencd.str());
    filencd<<".gz";
    remove(filencd.str().c_str());
    system(cmd.c_str());
    //cmd = gzip;
    //cmd.append(filebasin.str());
    //filebasin<<".gz";
    //remove(filebasin.str().c_str());
    //system(cmd.c_str());
    cmd = gzip;
    cmd.append(filebasincd.str());
    filebasincd<<".gz";
    remove(filebasincd.str().c_str());
    system(cmd.c_str());
    rememp2++;
    rememp2 = rememp2 % (largest+1);
    if(rememp2 == 0)rememp1++;
  }
  del(model);
  return EXIT_SUCCESS;
}
