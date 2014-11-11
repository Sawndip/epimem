#ifndef POPULATION_HPP
#define POPULATION_HPP

#include "model.hpp"

typedef struct Pop{
  int x;
  int y;
  Pottsmodel*** space;
  std::vector< std::pair<int,int> >* pos;
} Pop;

void init_pop(Pop* p, int x, int y, int l, int s);
void init_models(Pop* p, int p1, int p2, std::default_random_engine rng);
void init_interactions(Pop* p);
int mh_pop(Pop* p,std::default_random_engine rng, int upperbound);
void setCellfield(Pop* p);
void calcField(Pottsmodel* o, Pop* p, std::vector<std::pair<int,int> > n); 
void cleanUp(Pop* p, int errors, int pattern1, int pattern2);
int difference(int* string, int p, int l, int s);
void grow(Pop* p, std::default_random_engine rng);
void divide(Pottsmodel* p, Pottsmodel* child, std::default_random_engine rng);
void setState(int num, int* bin, int length, int states);
int** patternMatrix(Pop* p);
void del_pop(Pop* p);
#endif
