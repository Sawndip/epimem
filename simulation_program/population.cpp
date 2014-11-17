#include "population.hpp"

void init_pop(Pop* p, int x, int y, int l, int s, double ss, double lfs){
  int i,j;
  
  p->x = x;
  p->y = y;
  
  p->signalstrength = ss;
  p->localfieldstrength = lfs;


  p->pos = new std::vector<std::pair<int,int> > ();
  p->pos->reserve(x*y);
  
  p->space = (Pottsmodel***)malloc(x*sizeof(Pottsmodel**));
  for(i = 0; i < x; i++){
    p->space[i] = (Pottsmodel**)malloc(y*sizeof(Pottsmodel*));
    for(j = 0; j < y; j++){
      p->pos->push_back(std::make_pair(i,j));
      p->space[i][j] = (Pottsmodel*)malloc(sizeof(Pottsmodel));
      initialize(p->space[i][j], l, s);
    }
  }
}


void init_pop_growing(Pop* p, int x, int y, int l, int s, double ss, double lfs){
  int i,j;
  
  p->x = x;
  p->y = y;
  
  p->signalstrength = ss;
  p->localfieldstrength = lfs;


  p->pos = new std::vector<std::pair<int,int> > ();
  p->pos->reserve(x*y);
  
  p->space = (Pottsmodel***)malloc(x*sizeof(Pottsmodel**));
  for(i = 0; i < x; i++){
    p->space[i] = (Pottsmodel**)malloc(y*sizeof(Pottsmodel*));
    for(j = 0; j < y; j++){
      p->pos->push_back(std::make_pair(i,j));
    }
  }

  p->space[0][0]  = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(p->space[0][0], l, s);
  p->space[0][1]  = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(p->space[0][1], l, s);
  p->space[1][0]  = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(p->space[1][0], l, s);  
  p->space[p->x-1][p->y-1]  = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(p->space[p->x-1][p->y-1], l, s);
  p->space[p->x-1][p->y-2]  = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(p->space[p->x-1][p->y-2], l, s);
  p->space[p->x-2][p->y-1]  = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  initialize(p->space[p->x-2][p->y-1], l, s);

}

void init_models(Pop* p, int p1, int p2, std::default_random_engine rng){
  int i,j;
  for(i = 0; i < p->x; i++){
    for(j = 0; j < p->y; j++){
      //int pattern = rng() % 2;
      //if(pattern == 0){
      //	setState(p1,p->space[i][j]->modelstring, p->space[i][j]->length,p->space[i][j]->states); 
      //}else{
      //	setState(p2,p->space[i][j]->modelstring, p->space[i][j]->length,p->space[i][j]->states); 
      //}
      int pattern = p1;
      if(i > (p->x-1)/2) pattern = p2;
      setState(pattern,p->space[i][j]->modelstring, p->space[i][j]->length,p->space[i][j]->states);
    }
  }
}



void init_models_growing(Pop* p, int p1, int p2, std::default_random_engine rng){

  setState(p1,p->space[0][0]->modelstring, p->space[0][0]->length,p->space[0][0]->states);
  setState(p1,p->space[0][1]->modelstring, p->space[0][1]->length,p->space[0][1]->states);
  setState(p1,p->space[1][0]->modelstring, p->space[1][0]->length,p->space[1][0]->states);  
  setState(p2,p->space[p->x-1][p->y-1]->modelstring, p->space[p->x-1][p->y-1]->length,p->space[p->x-1][p->y-1]->states);
  setState(p2,p->space[p->x-1][p->y-2]->modelstring, p->space[p->x-1][p->y-2]->length,p->space[p->x-1][p->y-2]->states);
  setState(p2,p->space[p->x-2][p->y-1]->modelstring, p->space[p->x-2][p->y-1]->length,p->space[p->x-2][p->y-1]->states);

}

void init_interactions(Pop* p){
  int i,j,k,l;
  for(i = 0; i < p->x; i++){
    for(j = 0; j < p->y; j++){
      if(p->space[i][j] != NULL){
	for(k = 0; k < p->space[i][j]->length; k++){
	  for(l = 0; l < p->space[i][j]->length; l++){
	    if(abs(k-l) == 1){
	      if(p->space[i][j]->modelstring[k] == p->space[i][j]->modelstring[l])p->space[i][j]->interactions[k][l] = 1.0;
	      else p->space[i][j]->interactions[k][l] = -1.0;
	    }
	  }
	}
      }
    }
  }
}

int mh_pop(Pop* p,std::default_random_engine rng, int upperbound){
  int changes = 1;
  int i,j;
  int steps = 0; 
  while(changes > 0 && steps < upperbound){ 
    setCellfield(p);
    changes = 0;
    steps++;
    std::shuffle (p->pos->begin(), p->pos->end(), rng);
    // std::cerr<<"suffled"<<std::endl;
    for(i = 0; i < p->pos->size(); i++){
      int sitex = p->pos->at(i).first;
      int sitey = p->pos->at(i).second;
      //std::cerr<<i<<" "<<j<<" in grid"<<std::endl;
      if(p->space[sitex][sitey] != NULL){
	changes += mhStep(p->space[sitex][sitey], rng);
      }
    }
  }
  return changes;
}

void setCellfield(Pop* p){
  /* corners */
  Pottsmodel* corner = p->space[0][0];
  std::vector< std::pair<int,int> > neighbors;
  neighbors.reserve(9);
  neighbors.emplace_back(0,1);
  neighbors.emplace_back(1,0);
  neighbors.emplace_back(1,1);
  calcField(corner,p,neighbors);
  
  corner = p->space[0][p->y-1];
  neighbors.clear();
  neighbors.emplace_back(0,p->y-2);
  neighbors.emplace_back(1,p->y-1);
  neighbors.emplace_back(1,p->y-2);
  calcField(corner,p,neighbors);

  corner = p->space[p->x-1][0];
  neighbors.clear();
  neighbors.emplace_back(p->x-1,1);
  neighbors.emplace_back(p->x-2,0);
  neighbors.emplace_back(p->x-2,1);
  calcField(corner,p,neighbors);
  
  corner = p->space[p->x-1][p->y-1];
  neighbors.clear();
  neighbors.emplace_back(p->x-1,p->y-2);
  neighbors.emplace_back(p->x-2,p->y-1);
  neighbors.emplace_back(p->x-2,p->y-2);
  calcField(corner,p,neighbors);

  /* borders */
  int i,j;
  for(i = 1; i < p->x-1; i++){
    neighbors.clear();
    neighbors.emplace_back(i-1,0);
    neighbors.emplace_back(i+1,0);
    neighbors.emplace_back(i-1,1);
    neighbors.emplace_back(i,1);
    neighbors.emplace_back(i+1,1);
    calcField(p->space[i][0],p,neighbors);
  }

  for(i = 1; i < p->x-1; i++){
    neighbors.clear();
    neighbors.emplace_back(i-1,p->y-1);
    neighbors.emplace_back(i+1,p->y-1);
    neighbors.emplace_back(i-1,p->y-2);
    neighbors.emplace_back(i,p->y-2);
    neighbors.emplace_back(i+1,p->y-2);
    calcField(p->space[i][p->y-1],p,neighbors);
  }  

  for(i = 1; i < p->y-1; i++){
    neighbors.clear();
    neighbors.emplace_back(0,i-1);
    neighbors.emplace_back(0,i+1);
    neighbors.emplace_back(1,i-1);
    neighbors.emplace_back(1,i);
    neighbors.emplace_back(1,i+1);
    calcField(p->space[0][i],p,neighbors);
  }

  for(i = 1; i < p->y-1; i++){
    neighbors.clear();
    neighbors.emplace_back(p->x-1,i-1);
    neighbors.emplace_back(p->x-1,i+1);
    neighbors.emplace_back(p->x-2,i-1);
    neighbors.emplace_back(p->x-2,i);
    neighbors.emplace_back(p->x-2,i+1);
    calcField(p->space[p->x-1][i],p,neighbors);
  }
  
  /* intern */
  for(i = 1; i < p->x-1; i++){
    for(j = 1; j < p->y-1; j++){
      neighbors.clear();
      neighbors.emplace_back(i-1,j-1);
      neighbors.emplace_back(i-1,j);
      neighbors.emplace_back(i-1,j+1);
      neighbors.emplace_back(i,j-1);
      neighbors.emplace_back(i,j+1);
      neighbors.emplace_back(i+1,j-1);
      neighbors.emplace_back(i+1,j);
      neighbors.emplace_back(i+1,j+1);
      calcField(p->space[i][j],p,neighbors);
    }
  }
}

void calcField(Pottsmodel* o, Pop* p, std::vector<std::pair<int,int> > n){
  int i,j;
  if(o == NULL)return;
  for( i = 0; i < o->length; i++){
    double sum = 0.0;
    for(j = 0; j < n.size(); j++){
      if(p->space[n[j].first][n[j].second] != NULL)
	sum += (2.0*(double)p->space[n[j].first][n[j].second]->modelstring[i])-1.0;
    }
    o->cellfield[i] = p->signalstrength*sum/(double)n.size();
  }
}


void cleanUp(Pop* p, int errors, int pattern1, int pattern2){
  int i,j;
  
  for(i = 0; i < p->x; i++){
    for(j = 0; j < p->y; j++){
      if(p->space[i][j] != NULL){
	  int diffs1 = difference(p->space[i][j]->modelstring,pattern1,p->space[i][j]->length, p->space[i][j]->states);
	  int diffs2 = difference(p->space[i][j]->modelstring,pattern2,p->space[i][j]->length, p->space[i][j]->states);
	  if(diffs1 > errors && diffs2 > errors){
	    del(p->space[i][j]);
	    p->space[i][j] = NULL;
	  }
	}
    }
  }
}

int difference(int* string, int p, int l, int s){
  int diffs = 0;
  int i;
  for(i = 0; i < l; i++){
    int pstate = p%s;
    if(pstate != string[i])diffs++;
    p = (int)(p/s);
  }
  return diffs;
}

void grow(Pop* p, std::default_random_engine rng){
  int i,j;
  bool** settled = (bool**)malloc(p->x*sizeof(bool*));
  for(i = 0; i < p->x; i++){
    settled[i] = (bool*)malloc(p->y*sizeof(bool));
  }
  
  for(i = 0; i < p->x; i++){
    for(j = 0; j < p->y; j++){
      if(p->space[i][j] != NULL)settled[i][j] = true;
      else settled[i][j] = false;
    }
  }

  int k,l;
  //int lastk,lastl;
  for(i = 0; i < p->x; i++){
    for(j = 0; j < p->y; j++){
      if(settled[i][j]){
	//std::cerr<<"found that "<<i<<","<<j<<" is occupied"<<std::endl;
	std::vector<std::pair<int,int> > t;
	for(k = i-1; k <= i+1; k++){
	  for(l = j-1; l <= j+1; l++){
	    if((j != l || k != i) // not the current model
	       && k >= 0 && l >= 0 // not left of or above model
	       && k < p->x && l < p->y // not right of or below model
	       && p->space[k][l] == NULL){ //is free
	      t.emplace_back(k,l);
	    }
	  }
	}
	if(t.size() > 0){
	  std::pair<int,int> pair = t[rng() % t.size()];
	  //std::cerr<<"divide "<<i<<","<<j<<" to "<<pair.first<<" "<<pair.second<<std::endl;
	  p->space[pair.first][pair.second] = (Pottsmodel*)malloc(sizeof(Pottsmodel));
	  divide(p->space[i][j],p->space[pair.first][pair.second],rng,p->localfieldstrength);
	  //std::cerr<<"done"<<std::endl;
	  //int dummy;
	  //lastk = pair.first;
	  //lastl = pair.second;
	  //std::cerr<<p->space[pair.first][pair.second]<<std::endl;
	  //for(dummy = 0; dummy < p->space[pair.first][pair.second]->length; dummy++)std::cerr<<p->space[pair.first][pair.second]->modelstring[dummy];
	  //std::cerr<<std::endl<<std::endl;
	}
	
      }
    }
  }
  //std::cerr<<">>>"<<std::endl;
  //int dummy;
  //std::cerr<<p->space[lastk][lastl]<<std::endl;
  //for(dummy = 0; dummy < p->space[lastk][lastl]->length; dummy++)std::cerr<<p->space[lastk][lastl]->modelstring[dummy];
  //std::cerr<<std::endl<<std::endl;
  //
  //int** pM = patternMatrix(p);
  //for(i = 0; i < p->x; i++){
  //  std::cerr<<pM[i][0]<<" ";
  //  for(j = 1; j < p->y; j++){
  //    std::cerr<<" "<<pM[i][j];
  //  }
  //  std::cerr<<std::endl;
  //}
  //std::cerr<<"<<<"<<std::endl;

}

void divide(Pottsmodel* p, Pottsmodel* child, std::default_random_engine rng, double lf){
  // child = (Pottsmodel*)malloc(sizeof(Pottsmodel));
  //std::cerr<<child<<" "<<p->length<<" "<<p->states<<std::endl;
  initialize(child,p->length,p->states);
  /* copy interactions and set local field */
  child->interactions = (double**)malloc(child->length*sizeof(double*));
  int i,j;
  for(i = 0; i < child->length; i++){
    child->interactions[i] = (double*)malloc(child->length*sizeof(double));
    for(j = 0; j < child->length; j++){
      child->interactions[i][j] = p->interactions[i][j];
    }
  }
  
  child->localfield = (double*)malloc(child->length*sizeof(double));
  for(i = 0; i < child->length; i++){
    child->localfield[i] = lf*(2.0*p->modelstring[i]-1);
    p->localfield[i] = lf*(2.0*p->modelstring[i]-1);
  }
  /* reserve space for cell field */
  child->cellfield = (double*)malloc(child->length*sizeof(double));
  /* half-copy modelstring */
  child->modelstring = (int*)malloc(child->length*sizeof(int));
  for(i = 0; i < p->length; i++){
    int random = rand();
    //std::cerr<<"dev rand "<<random<<std::endl;
    random = random%2;
    //std::cerr<<"dev rand "<<random<<std::endl;
    //std::cerr<<" parent: "<<p->modelstring[i]<<std::endl;
    if(random == 1){
      child->modelstring[i] = 0;
    }else{
      //std::cerr<<"set to "<< p->modelstring[i] <<"--";
      child->modelstring[i] = p->modelstring[i];
      p->modelstring[i] = 0;
    }
    //std::cerr<<"child: "<<child->modelstring[i]<<std::endl;
  } 
  //std::cerr<<std::endl;
  //for(i = 0; i < p->length; i++)std::cerr<<child->modelstring[i];
  //std::cerr<<std::endl;
}

void setState(int num, int* bin, int length, int states){
  int i;
  for(i = 0; i < length; i++){
    bin[i] = num % states;
    num = (int)(num/states);
  }
  
}

int** patternMatrix(Pop* p){
  int i,j;
  int** patMa = (int**)malloc(p->x*sizeof(int*));
  for(i = 0; i < p->x; i++){
    patMa[i] = (int*)malloc(p->y*sizeof(int));
    for(j = 0; j < p->y; j++){
      if(p->space[i][j] == NULL)patMa[i][j] = -1;
      else patMa[i][j] = nary2int(p->space[i][j]);
    }
  }
  return patMa;
}

void del_pop(Pop* p){
  //p->pos.erase(p->pos.begin(),p->pos.end());
  int i,j;
  for(i = 0; i < p->x; i++){
    for(j = 0; j < p->y; j++){
      if(p->space[i][j] != NULL)del(p->space[i][j]);
    }
    free(p->space[i]);
  }
  free(p->space);
  delete p->pos;
}
