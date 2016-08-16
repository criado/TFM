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
inline void nextElement(mask f, mask& x) {x= ((x|(~f))+x)&f;}

// mask x=0; do{ stuff } while(x=nextSubset(f,x), x!=0)
inline void  nextSubset(mask f, mask& x) {x= ((x|(~f))+1)&f;}

void printMask(mask f) {cout<<bitset<32>(f)<<endl;}
mask readMask() {
  string str; getline(cin, str); mask res=0;
  for(int i=0; i<str.size(); i++) res=2*res+((str[i]=='1')?1:0);
  return res;
}

inline bool in(mask& a, mask&b) {return !(a&(~b));}

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
    for(mask x=firstElement(f); x!=0; nextElement(f,x)) {
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
    for(mask x=firstElement(it.first); x!=0; nextElement(it.first, x)) {
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
flip Prismatoid::findFlip() {
  flip fl;
  uniform_int_distribution<unsigned long long> dis(0, options.size()-1);
  auto origin = options.begin(); advance(origin, dis(generator));

  for(auto it= origin; it!= options.end(); ++it)
    if(checkFlip(it->first, fl)) return fl;
  for(auto it=options.begin(); it!=origin; ++it)
    if(checkFlip(it->first, fl)) return fl;

  assert(false); return fl;
}

// Makes the flip (f,l,v).
void Prismatoid::execFlip(flip fl) {
  queue<mask> q; mask f=fl.f, l=fl.l, v=fl.v, u=f|l|v;
  // Note that the star (within the support) for a new face is:
  // -If it has exactly one 0 in f, the support without that zero
  // -If it has more than one 0 in f, u

  mask x=0; do {

    // The forbidden facet
    if(x==(f|l)) continue;

    // The supersets of f are disappearing faces.
    if(in(f,x)) {
      if(countBits(x)==dim) dists.erase(x);
      if(countBits(x)==dim-1) {
        if(in(x,base2)) adyBase2.erase(x);
        if(--options[SC[x]]==0) options.erase(SC[x]);
      }
      SC.erase(x);
    }

    // The supersets of l are appearing faces.
    else if(in(l,x)) {
      SC[x]= (countBits(f&~x)==1)? ((f&~x)|l|v): u;
      if(countBits(x)==dim) q.push(x);
      if(countBits(x)==dim-1) {
        if(in(x,base2)) adyBase2.insert(x);
        ++options[SC[x]];
    } }
    
    // Stuff that remain the same.
    else { 
      if(countBits(x)==dim-1) if(--options[SC[x]]==0) options.erase(SC[x]);
      SC[x]&=~u; SC[x]|= (countBits(f&~x)==1)? ((f&~x)|l|v): u;
      if(countBits(x)==dim-1) ++options[SC[x]];
    }

  } while(nextSubset(u,x), x!=u); // TODO behaviour as expected?

  base1=SC[0]&LAYER1; base2=SC[0]&LAYER2; updateDists(q);
}

// Makes a random move. Returns its (f,l,v).
flip Prismatoid::execFlip() { flip fl=findFlip(); execFlip(fl); return fl;}

// Allow a move if
// - It is an option (assumed)
// - There's room to add a new vertex.
// - The corresponding l is not in the complex.
// - It does not change the number of vertices (under changeBases)
// Returns the flip by reference.
bool Prismatoid::checkFlip(mask u, flip& fl) {

  if(countBits(u)==dim) { // Add a vertex to the support when required.
    mask newv, LAYER;

    if     (countBits(u&LAYER2)==1) LAYER=LAYER1;
    else if(countBits(u&LAYER1)==1) LAYER=LAYER2;
    else {cerr<<"Error 841: Not enough cheese in buffer."<<endl; return false;}
    
    if((newv=firstElement(LAYER & ~u))==0) return false;
    u|=newv;
  }
  fl.f=u;
  for(mask x=firstElement(u); x!=0; nextElement(u,x))
    if(SC.find(u^x)!=SC.end()) fl.f^=x;
  fl.l=u^fl.f;
  if( countBits(u&base1)==1) fl.v=(u&base1), fl.f^=fl.v; 
  if( countBits(u&base2)==1) fl.v=(u&base2), fl.f^=fl.v;

  // l must not be in SC.
  if(SC.find(fl.l)!=SC.end()) return false;

  // Am I adding/removing a vertex?
  if(!changeBases) if(countBits(fl.l)==1) return false;
  if(!changeBases) if(countBits(fl.f)==1) return false;

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
      else if((LAYER2 & it.first)==0) q.push(it.second);
  } }

  dists=map<mask,ii>(); updateDists(q);
}


// Stuff for the computation of the diameter and width.
inline void relaxPair(ii& me, ii& other) {
  if(other.first+1<me.first)       me.first = other.first+1,
                                   me.second= other.second;
  else if(other.first+1==me.first) me.second+=other.second;
}
void Prismatoid::updateDists(queue<mask>& q) {
  while(!q.empty()) {
    mask f=q.front(), f2; q.pop();

    if(countBits(base1&f)==dim-1) {
      if(dists[f]==ii(1,1)) continue;
      dists[f]=ii(1,1);

      for(mask x= firstElement(f); x!=0; nextElement(f,x))
        if(countBits(f2=SC[f^x]^x)==dim) q.push(f2);
    }
    else {
      // if(dists[f]==ii(0,0)) dists[f]=ii(201,0); // needed?
      ii aux=ii(200,0);
      for(mask x= firstElement(f); x!=0; nextElement(f,x)) {
        if(countBits(f2=SC[f^x]^x)!=dim) continue;
        relaxPair(aux, dists[f2]);
      }
      if(aux!=dists[f]) {
        dists[f]=aux;
        for(mask x=firstElement(f); x!=0; nextElement(f,x))
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
