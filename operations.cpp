#include "operations.h"
#include "complex.h"
#include <queue>

uint base1,base2;
map<uint,uint> SC;
map<uint, ii> graph;
set<uint> options;
queue<int> q;

////////////////////////////////////////////////////////////////////////////////
// Plumber's headers
////////////////////////////////////////////////////////////////////////////////
inline uint countBits(uint i); 

void addFacet(uint f);
void initOptions();
void initDiameter();

bool isFlip(uint u);
inline void setmin(ii&a, ii b) {
  if(b.first<a.first) {a.first=b.first; a.second=b.second;}
  else if(b.first==a.first) {a.second+=b.second;}
}
void relax(uint f); //auxiliar function for diameter;


////////////////////////////////////////////////////////////////////////////////
// Interesting stuff
////////////////////////////////////////////////////////////////////////////////
void crosspolytope() {
  base1=((1<<dim)-1)<<n; base2=(1<<dim)-1;
  SC=map<uint,uint>();   options=set<uint>();

  for(uint i=0; i<(1<<dim); i++) {
    if(i!=0 && i!=base2) {
      addFacet(i|((base2^i)<<dim));
    }
  }
  initOptions(); initDiameter();
}

uint findFlip(); //Gets a valid random flip

void flip(uint u); //flips or unflips. Updates graph too



pair<int, ii> costs() {
  return make_pair(countBits(base1|base2), graph[base2]);
}

pair<vi, vi> statisticsForSantos() {
  vi fvector(dim+1), layers(dim+2);
  for (auto it=SC.begin(); it!=SC.end();++it) fvector[countBits(it->second)]++;
  for (auto it=graph.begin(); it!=graph.end();++it) layers[it->second.second]++;

  return make_pair(fvector, layers);
}

////////////////////////////////////////////////////////////////////////////////
// Plumber's stuff
////////////////////////////////////////////////////////////////////////////////
inline uint countBits(uint i) {
  i= i-((i>>1)&0x55555555); i= (i&0x33333333)+((i>>2)&0x33333333);
  return (((i+(i>>4))&0x0F0F0F0F)*0x01010101) >> 24;
}

inline uint nextSubset(uint f, uint x) {
  return (((((1<<2*n)-1)^f)|x)+1)&f;
}

//Sloooow
void addFacet(uint f) {
  graph[f]=ii(200,0); uint x=0; do SC[x]|=f, x=nextSubset(f,x); while(x!=0);
}

void initOptions() {
  options=set<uint>();

  for(auto it=SC.begin(); it!=SC.end(); ++it) {
    uint f=it->first, u=it->second;

    if((f|base1) && (f|base2) && countBits(f)==dim-1)
      options.insert(u);

    if(f==u && countBits(base1&f)==1 && countBits(base2&f)==1)
      options.insert(f|(1<<(2*n)));
  }
}

void initDiameter() {
  graph= map<uint,ii>(); q= queue<int>();
  graph[base1]=ii(0,1); graph[base2]=ii(200,0);

  for(auto it=SC.begin(); it!=SC.end(); ++it) {
    if(countBits(it->first)==dim && countBits(base1&(it->first))==dim-1)
      q.push(it->first);
  }

  while(!q.empty()) {relax(q.front()); q.pop();}
}

bool isFlip(uint u) { //TODO
  return true;
}

void relax(uint f) { //TODO
  ii aux(200,0);
  for(uint l=f;l!=0; l^=(l&-l)) {
    uint ridge=f^(l&-l), f2=ridge|(SC[ridge]^f);
    
  }
  
}

void readPrismatoid(string path); //gets a prismatoid from file

void writePrismatoid(string path); //prints a prismatoid to file


