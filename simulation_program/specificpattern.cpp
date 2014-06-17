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
  if(argc < 7){
    std::cerr<<"./specificpattern <outputprefix> <length> <number of pattern to calculate> <calcpatttern_1> [<calcpattern_2> ... <calcpattern_n>] <pattern_1> [<pattern_2> ... <pattern_n>] <graph file>"<<std::endl;
    return EXIT_FAILURE;
  }
  int length;
  int calcl;
  std::string graph(argv[argc-1]);
  argc--;
  std::vector<int> patterns;
  std::vector<int> calcpatterns;
  std::istringstream s(argv[2]);
  s >> length;
  std::istringstream calc(argv[3]);
  calc >> calcl;
  calcpatterns.reserve(calcl); 
  patterns.reserve(argc - 4 - calcl);  
  int a;
  for(a = 4; a < argc; a++){
    std::string str(argv[a]);
    //std::cerr<<str<<std::endl;
    std::istringstream pat(str);
    int p;
    pat >> p;
    if(a < (calcl + 4)){ 
      calcpatterns.push_back(p);
    }else{
      patterns.push_back(p);
    }
    //std::cerr<<"pattern="<<p<<std::endl;
  }

  Pottsmodel* model = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(model,length,2);
  unsigned int i,j,t;
  std::string filename(argv[1]);
  //std::ostringstream fileno;
  //fileno<<filename<<".original";
  std::ostringstream filencd;
  filencd<<filename<<"_"<<calcpatterns[0]<<".celldivision";
  //std::ostringstream filebasin;
  //filebasin<<filename<<".basins";;
  ////std::ostringstream filebasincd;
  ////filebasincd<<filename<<"_"<<calcpatterns[0]<<".basinscd";
  //std::ofstream org;
  //org.open(fileno.str().c_str());
  std::ofstream cd;
  cd.open(filencd.str().c_str());
  //std::ofstream basins;
  //basins.open(filebasin.str().c_str());
  ////std::ofstream basinscd;
  ////basinscd.open(filebasincd.str().c_str());

  /* initialize with 0.0 */
  for(i = 0; i < model->length; i++){
    for(j = 0; j < model->length; j++){
      model->interactions[i][j] = 0.0;
    }
  }
  
  /* set weights according to the full graph */
  double weight = 1.0/(double)patterns.size();
  for(a = 0; a < patterns.size(); a++){
    int2nary(patterns[a],model->modelstring,model->length,model->states);
    for(i = 0; i < model->length; i++){
      for(j = 0; j < model->length; j++){
	if(i != j)
	  if(model->modelstring[i] == model->modelstring[j])model->interactions[i][j] += weight;
	  else model->interactions[i][j] -= weight;
      }
    }
  }


  /* generate or load erdoes renyi graph and remove edges not in this graph */
  std::ifstream ifile;
  ifile.open(graph.c_str());
  if(ifile.is_open()){
    int readlength = -1;
    double val;
    /* length handling ==> same length required */
    ifile >> readlength;
    if(readlength != model->length){
      std::cerr<<"model and graph file are not equally long!\n";
      del(model);
      ifile.close();
      cd.close();
      ////      basinscd.close();
      return EXIT_FAILURE;
    }
    for(i = 0; i < model->length; i++){
      for(j = 0; j < model->length; j++){
	ifile >> val;
	if(val == 0.0){
	  model->interactions[i][j] = 0.0;
	}
      }
    }
  }else{
    double erthreshold = log((double)model->length)/(double)model->length + 0.1; // + 0.1 for connected
    for(i = 0; i < model->length; i++){
      for(j = i+1; j < model->length; j++){
	double include = (double)rand()/(double)RAND_MAX;
	if(include >= erthreshold){
	  model->interactions[i][j] = 0.0;
	  model->interactions[j][i] = 0.0;
	}
      }
    }
    /* print it */
    std::ofstream outgraph;
    outgraph.open(graph.c_str());
    
    outgraph<<model->length<<std::endl;
    for(i = 0; i < model->length; i++){
      for(j = 0; j < model->length; j++){
	outgraph<<model->interactions[i][j]<<"\t";
      }
      outgraph<<std::endl;
    }
    outgraph.close();
  }
  
  



  /* do monte carlo simulation to find the distribution of the states */
  //TODO
  //std::map<int,int> states;
  
  int samples = 1000;
  int upbou = 10000;
  int max = pow(model->states,model->length);
  int b;
  //int patternssize = patterns.size();
  //for(b = 0; b < patternssize; b++){
  //  patterns.push_back((int)pow(model->states,model->length)-1-patterns[b]);
  //}
 //for(b = 0; b < patterns.size(); b++){
 //  int unstable = 0;
 //  int diff = 0;
 //  int same = 0;
 //  for(i = 0; i < samples; i++){
 //    int2nary(patterns[b],model->modelstring,model->length,model->states);
 //    int changes = mh(model,rng,upbou);
 //    int curstate = nary2int(model->modelstring,model->length,model->states);
 //    if(changes > 0){
 //	unstable++;
 //    }else{
 //	if(curstate == patterns[b]){
 //	  same++;
 //	}else{
 //	  diff++;
 //	}
 //    }    
 //  }
 //  org<<patterns[b]<<"\t"<<same<<"\t"<<diff<<"\t"<<unstable<<std::endl; 
 //}
 ///* basins of original model */
 //for(b = 0; b < (int)pow(model->states,model->length); b++){
 //  for( i = 0; i < samples; i++){
 //    int2nary(b,model->modelstring,model->length,model->states);
 //    int changes = mh(model,rng,upbou);
 //    int curstate = nary2int(model->modelstring,model->length,model->states);
 //    if(changes == 0)basins<<b<<"\t"<<curstate<<std::endl;
 //  }
 //}




  
  int* init = (int*)malloc(sizeof(int)*model->length);
  //std::cout<<"#CELLDIVISION"<<std::endl;
  for(b = 0; b < calcpatterns.size(); b++){
    int backtoorigin = 0;
    int converttodifferent = 0;
    int unstable = 0;
    int2nary(calcpatterns[b],init,model->length,model->states);
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
	  ////basinscd<<calcpatterns[b]<<"\t"<<celldivstate<<"\t"<<res<<std::endl;
	  if(calcpatterns[b] == res)backtoorigin++;
	  else converttodifferent++;
	}
      }
    }
    cd<<calcpatterns[b]<<"\t"<<backtoorigin<<"\t"<<converttodifferent<<"\t"<<unstable<<std::endl; 
  }
  //org.close();
  cd.close();
  //basins.close();
  //// basinscd.close();
  /* gzip files */
  std::string gzip("gzip ");
  std::string cmd = gzip;
  //cmd.append(fileno.str());
  //fileno<<".gz";
  //remove(fileno.str().c_str());
  //system(cmd.c_str()); 
  cmd = gzip;
  cmd.append(filencd.str());
  filencd<<".gz";
  remove(filencd.str().c_str());
  system(cmd.c_str());
  //cmd = gzip;
  //cmd.append(filebasin.str());
  //filebasin<<".gz";
  //remove(filebasin.str().c_str());
  //system(cmd.c_str());
  ////cmd = gzip;
  ////cmd.append(filebasincd.str());
  ////filebasincd<<".gz";
  ////remove(filebasincd.str().c_str());
  ////system(cmd.c_str());
  del(model);
  return EXIT_SUCCESS;
}
