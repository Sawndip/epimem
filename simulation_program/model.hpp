#ifndef MODEL_HPP
#define MODEL_HPP
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>    // std::move_backward
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

typedef struct Pottsmodel {
  int length;
  int states;
  double** interactions;
  int* modelstring;
  double* localfield;
  double* cellfield;
  std::vector<int>* pos;
} Pottsmodel;

void initialize(Pottsmodel* p, int l, int s);
int update(Pottsmodel* p);
int mhStep(Pottsmodel* p,std::default_random_engine rng);
int mh(Pottsmodel* p,std::default_random_engine rng, int upperbound);
void del(Pottsmodel* p);
int nary2int(Pottsmodel* p);
#endif
