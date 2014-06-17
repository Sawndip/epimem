#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <map>
#include <vector>
#include <cmath>




void int2nary(int num, int* bin, int length, int states){
  int i;
  for(i = 0; i < length; i++){
    bin[i] = num % states;
    num = (int)(num/states);
  }
  
}




int main(int argc, char* argv[]){

  if(argc != 3){
    std::cerr<<"./graph2pattern <graph file> <output file>"<<std::endl;
    return EXIT_FAILURE;
  }
  int length;
  std::string graph(argv[1]);
  std::string outfile(argv[2]);

  std::ofstream out;
  out.open(outfile.c_str());

  double** matrix;
  int* model;
  unsigned int i,j;

  std::ifstream ifile;
  ifile.open(graph.c_str());
  if(ifile.is_open()){
    double val;    
    ifile >> length;
    
    matrix = (double**)malloc(length*sizeof(double*));
    for(i = 0; i < length; i++)matrix[i] = (double*)malloc(length*sizeof(double));

    model = (int*)malloc(length*sizeof(int));

    for(i = 0; i < length; i++){
      for(j = 0; j < length; j++){
	ifile >> val;
	matrix[i][j] = val;
      }
    }
  }else{
    std::cerr<<"Could not open graph file"<<std::endl;
    return EXIT_FAILURE;
  }
  ifile.close();

  int max = (unsigned int)pow(2,length);
  unsigned int p;
   /*
  for(p = 0; p < max; p++){
    int2nary(p,model,length,2);
    double prob = 1.0;
    for(i = 0; i < length; i++){
      for(j = i+1; j < length; j++){
	double sameprob = (matrix[i][j] + 1.0)/2.0;
	if(model[i] !=  model[j])prob *= (1.0 - sameprob);
	else prob *= sameprob;
      }
    }
    out << p << "\t" << prob <<std::endl;
  }
  */

  double partitionfunction = 0.0;
  for(p = 0; p < max; p++){
    int2nary(p,model,length,2);
    double h = 0.0;
    for(i = 0; i < length; i++){
      double state1 = -1.0;
      if(model[i] == 1)state1 = 1.0;
      for(j = i+1; j < length; j++){
	double state2 = -1.0;
	if(model[i] == 1)state2 = 1.0;
	h += state1*state2*matrix[i][j];
      }
    }
    partitionfunction += exp(h);
  }
  
    
  for(p = 0; p < max; p++){
    int2nary(p,model,length,2);
    double h = 0.0;
    for(i = 0; i < length; i++){
      double state1 = -1.0;
      if(model[i] == 1)state1 = 1.0;
      for(j = i+1; j < length; j++){
	double state2 = -1.0;
	if(model[i] == 1)state2 = 1.0;
	h += state1*state2*matrix[i][j];
      }
    }
    out<<p<<"\t"<<exp(h)/partitionfunction<<std::endl;
  }
  

  out.close();
 
  return EXIT_SUCCESS;
}
