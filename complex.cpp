#include "complex.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// S0: Ancient bit-jutsu techniques
////////////////////////////////////////////////////////////////////////////////
inline uint countBits(mask i) {
  i= i-((i>>1)&0x55555555); i= (i&0x33333333)+((i>>2)&0x33333333);
  return (((i+(i>>4))&0x0F0F0F0F)*0x01010101) >> 24;
}

// for (mask x=firstElement(f); x!=0; x=nextElement(f,x))
inline mask firstElement(mask f) {return f&(-f);}
inline mask nextElement(mask f, mask x) {return ((x|(~f))+x)&f;}

// mask x=0; do{ stuff } while(x=nextSubset(f,x), x!=0)
inline mask  nextSubset(mask f, mask x) {return ((x|(~f))+1)&f;}

void printMask(mask f) {cout<<bitset<32>(f)<<endl;}
mask readMask() {
  string str; getline(cin, str); mask res=0;
  for(int i=0; i<str.size(); i++) res=2*res+((str[i]=='1')?1:0);
  return res;
}

////////////////////////////////////////////////////////////////////////////////
// S1: Constructors and IO
////////////////////////////////////////////////////////////////////////////////

// Assumes that in SC we have only facets of dim dim. Builds everything else.
// You should only call this in a constructor, or bad things could happen.
void Prismatoid::cascadeFacets() {
  queue<mask> q;

  for(auto& it: SC) q.push(it.first),
                    base1|= LAYER1 & it.first,
                    base2|= LAYER2 & it.first;

  while(!q.empty()) {
    mask f=q.front(); q.pop();
    for(mask x=firstElement(f); x!=0; x=nextElement(f,x)) {
      SC[f^x]|=f; q.push(f^x);
  } }

  initOptions(); initGraph();

  unsigned seed= chrono::system_clock::now().time_since_epoch().count();
  generator=default_random_engine(seed);
}

// Crosspolytope constructor
Prismatoid::Prismatoid(int _dim) {
  dim=_dim; SC=map<uint,uint>();
  for(uint i=1; i<(1<<dim)-1; i++) SC[i|((base2^i)<<N)]=i|((base2^i)<<N);

  cascadeFacets();
}

// Constructor reading from file. The format is:
// -first line: dim, number of facets
// -next lines: each line is a new facet.
// The vertices 0..N-1 are in base2 and N..2N-1 in base1 (N=10).
Prismatoid::Prismatoid(istream& input) {
  SC=map<mask,mask>(); dists=map<mask, ii>();

  int numFacets; input>>dim>>numFacets;
  
  for(int i=0; i<numFacets; i++) {
    mask f=0, aux; for(int j=0; j<dim; j++) input>>aux, f|=(1<<aux);
    SC[f]=f;
  }
  cascadeFacets();
}

// Write prismatoid to file in the same format.
void Prismatoid::write(ostream& output) {
  int numFacets=0; for(auto& it: SC) if(countBits(it.first)==dim) ++numFacets;

  output<<dim<<" "<<numFacets<<endl;
  for(auto& it: SC) if(countBits(it.first)==dim)
    for(mask x=firstElement(it.first); x!=0; x=nextElement(it.first, x)) {
      for(int y=0; y<2*N; ++y) if(x==(1<<y)) output<<y<<" ";
      output<<endl;
}   }

////////////////////////////////////////////////////////////////////////////////
// S2: Flippin' magic.
////////////////////////////////////////////////////////////////////////////////

// Inits the option set.
void Prismatoid::initOptions() {
  options=map<mask,int>();;
  for(auto &it:SC) if(countBits(it.first)==dim-1) options[it.second]++;
}

// Finds a move or crashes tryin'.
pair<mask,mask> Prismatoid::findMove() {
  pair<mask,mask> res;
  uniform_int_distribution<unsigned long long> dis(0, options.size()-1);
  auto origin = options.begin(); advance(origin, dis(generator));

  for(auto it= origin; it!= options.end(); ++it)
    if(check(it->first, res)) return res;
  for(auto it=options.begin(); it!=origin; ++it)
    if(check(it->first, res)) return res;

  assert(false); return res;
}

// Make the flip (f,l). TODO
// Note that for frontier flips, there are two valid representations
void Prismatoid::move(pair<mask,mask> fl) {
  queue<mask> q;
  mask frontierFacet=0; // The facet that should not be added.
  mask f=fl.first, l=fl.second, u=f|l;

  // Canonincally, f should intersect both layers.
  if(countBits(base1&u)==1) {
    if((f&base1)==0) f|=base1&u, l^=base1&u;
    frontierFacet=u^(LAYER1&u);
  }
  if(countBits(LAYER2&u)==1) {
    if((f&LAYER2)==0) f|=(LAYER2&u), l^=(LAYER2&u);
    frontierFacet=u^(LAYER2&u);
  }

  mask x=0;
  do { //Iterate through every subset

    // f subset of x, this face will disappear
    if(!((f&frontierFacet)&~x)) {
      SC.erase(x);

      if(countBits(x)==dim-1) {
      
      }
      if(countBits(x)==dim) {
      
      }
    }

    // x superset of l, and not subset of the frontier facet.
    // This face will appear.
    else if(!(l&~x) && !(x&~frontierFacet)) {
    
    }

    //this facet will remain the same.
    else {
    
    }

  } while(x= nextSubset(u,x), x!=u);
}

// Makes a random move. Returns its (f,l).
pair<mask,mask> Prismatoid::move() {
  pair<mask,mask> fl=findMove(); move(fl); return fl;
}

// Allow a move if
// - It is an option (assumed)
// - There's room to add a new vertex.
// - The corresponding l is not in the complex.
// - It does not change the number of vertices (under changeBases)
// Returns (by reference) the pair (f,l) of the flip.
// Note that f must intersect both layers by definition.
bool Prismatoid::check(mask u, pair<mask,mask>& fl) {
  if(countBits(u)==dim) { // Add a vertex to the support when required.
    mask newv, LAYER;

    if     (countBits(u&LAYER2)==1) LAYER=LAYER1;
    else if(countBits(u&LAYER1)==1) LAYER=LAYER2;
    else {cerr<<"Error 841: Not enough cheese in buffer."<<endl; return false;}
    
    if((newv=firstElement(LAYER & ~u))==0) return false;
    u|=newv;
  }
  fl.first=u;
  for(mask x=firstElement(u); x!=0; x=nextElement(u,x))
    if(SC.find(u^x)!=SC.end()) fl.first^=x;
  fl.second=u^fl.first;

  if(SC.find(fl.second)!=SC.end()) return false;

  // Am I adding a vertex?
  if(!changeBases) if(countBits(fl.second)==1) return false;

  // Am I removing a vertex?
  if(!changeBases) if(countBits(u & LAYER1)==1 || countBits(u & LAYER2)==1)
    if(countBits(fl.first)==2) return false;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// S3: Costs and graph stuff
////////////////////////////////////////////////////////////////////////////////

// Inits graph and dists
void Prismatoid::initGraph() {
  adyBase2=set<mask>(); queue<mask> q;

  for(auto &it:SC) {
    if(countBits(it.first)==dim-1) {
      if     ((LAYER1 & it.first)==0) adyBase2.insert(it.first);
      else if((LAYER2 & it.first)==0)q.push(it.first);
  } }

  dists=map<mask,ii>(); updateCosts(q);
}


// Stuff for the computation of the diameter and width.
inline void relaxPair(ii& me, ii& other) {
  if(other.first+1<me.first)       me.first = other.first+1,
                                   me.second= other.second;
  else if(other.first+1==me.first) me.second+=other.second;
}
void Prismatoid::updateCosts(queue<mask>& q) {
  while(!q.empty()) {
    mask f=q.front(), f2; q.pop();

    if(countBits(base1&f)==dim-1) {
      if(dists[f]==ii(1,1)) continue;
      dists[f]=ii(1,1);

      for(mask x= firstElement(f); x!=0; x=nextElement(f,x))
        if(countBits(f2=SC[f^x]^x)==dim) q.push(f2);
    }
    else {
      ii aux=ii(200,0);
      for(mask x= firstElement(f); x!=0; x=nextElement(f,x)) {
        if(countBits(f2=SC[f^x]^x)!=dim) continue;
        relaxPair(aux, dists[f2]);
      }
      if(aux!=dists[f]) {
        dists[f]=aux;
        for(mask x=firstElement(f); x!=0; x=nextElement(f,x))
          if(countBits(f2=SC[f^x]^x)==dim) q.push(f2);
} } } }

// Number of vertices, distance and width
pair<int, ii> Prismatoid::costs() {
  ii aux(200,0);
  for(auto &it: adyBase2) relaxPair(aux, dists[it]);
  return make_pair(countBits(base1|base2), aux);
}

// f-vector and layers.
pair<vi, vi> Prismatoid::statsForSantos() {
  vi fvector(dim+1), layers(dim+2);
  for(auto &it: SC) fvector[countBits(it.first)]++;
  for(auto &it: dists) layers[it.second.second]++;

  return make_pair(fvector, layers);
}

////////////////////////////////////////////////////////////////////////////////
// S4: Don't panic. Please don't panic.
////////////////////////////////////////////////////////////////////////////////

// Can I panic now?
bool Prismatoid::everythingIsOK() {
  // 1: is the simplicial complex a prismatoid? (not easy).


  // 2: is the graph correct?
  // Every vertex of the dual graph should have the same degree.
  
  return true;
}
