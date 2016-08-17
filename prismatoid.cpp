#include "prismatoid.hpp"

////////////////////////////////////////////////////////////////////////////////
// S0: Ancient bit-jutsu techniques
////////////////////////////////////////////////////////////////////////////////
inline uint countBits(mask i) {
  i= i-((i>>1)&0x55555555); i= (i&0x33333333)+((i>>2)&0x33333333);
  return (((i+(i>>4))&0x0F0F0F0F)*0x01010101) >> 24;
}

// for (mask x=firstElement(f); x!=0; x=nextElement(f,x))
inline mask firstElement(mask f) {return f&-f;}
inline void nextElement(mask f, mask& x) {x= ((x|~f)+x)&f;}

// mask x=0; do{ stuff } while(x=nextSubset(f,x), x!=0)
inline void  nextSubset(mask f, mask& x) {x= ((x|~f)+1)&f;}

void printMask(mask f) {
  for(int i=0; i<2*N; i++) if(((1<<i)&f)!=0) cout<<i<<" "; cout<<endl;
}
mask readMask() {
  string str; getline(cin, str); mask res=0;
  for(int i=0; i<str.size(); i++) res=2*res+((str[i]=='1')?1:0);
  return res;
}

inline bool in(mask a, mask b) {return !(a&(~b));}

////////////////////////////////////////////////////////////////////////////////
// S1: Constructors and IO
////////////////////////////////////////////////////////////////////////////////

// Assumes that in SC we have only facets of dim dim. Builds everything else.
void prismatoid::cascadeFacets() {
  queue<mask> q; base1=base2=0;

  for(auto& it: SC) q.push(it.first),
                    base1|= LAYER1 & it.first,
                    base2|= LAYER2 & it.first;

  while(!q.empty()) {
    mask f=q.front(); q.pop();
    for(mask x=firstElement(f); x!=0; nextElement(f,x)) {
      SC[f^x]|=f; q.push(f^x);
  } }

  initOptions(); initGraph();
  assert(everythingIsOK());

  unsigned seed= chrono::system_clock::now().time_since_epoch().count();
  //generator=default_random_engine(seed);
  generator=default_random_engine(142857);
  cout<<"built"<<endl;
}

// Crosspolytope constructor
prismatoid::prismatoid(int _dim) {
  dim=_dim; SC=map<uint,uint>(); mask f; base2=(1<<dim)-1; base1=base2<<N;

  for(uint i=1; i<base2; i++) f=(i|((i<<N)^base1)), SC[f]=f;
  cascadeFacets();
}

// Constructor reading from file. The format is:
// -first line: dim, number of facets
// -next lines: each line is a new facet.
// The vertices 0..N-1 are in base2 and N..2N-1 in base1 (N=10).
prismatoid::prismatoid(istream& input) {
  SC=map<mask,mask>();

  int numFacets; input>>dim>>numFacets;
  
  for(int i=0; i<numFacets; i++) { //readFacet?
    mask f=0, aux; for(int j=0; j<dim; j++) input>>aux, f|=(1<<aux);
    SC[f]=f;
  }
  cascadeFacets();
}

// Write prismatoid to file in the same format.
void prismatoid::write(ostream& output) {
  int numFacets=0; for(auto& it: SC) if(countBits(it.first)==dim) ++numFacets;

  output<<dim<<" "<<numFacets<<endl;
  for(auto& it: SC) if(countBits(it.first)==dim) {
    for(int i=0; i<2*N; ++i) if((it.first&(1<<i))!=0) output<<i<<" ";
    
    output<<endl;
  }
}

////////////////////////////////////////////////////////////////////////////////
// S2: Flippin' magic.
////////////////////////////////////////////////////////////////////////////////

// Inits the option set.
void prismatoid::initOptions() {
  options=map<mask,int>();
  for(auto &it:SC) if(countBits(it.first)==dim-1) options[it.second]++;
}

// Finds a move or crashes tryin'.
flip prismatoid::findFlip() {
  flip fl;
  uniform_int_distribution<unsigned long long> dis(0, options.size()-1);
  auto origin = options.begin(); advance(origin, dis(generator));

  for(auto it= origin; it!= options.end(); ++it)
    if(checkFlip(it->first, fl)) return fl;
  for(auto it=options.begin(); it!=origin; ++it)
    if(checkFlip(it->first, fl)) return fl;

  assert(false); return fl;
}

int numflips=0;
// Makes the flip (f,l,v).
void prismatoid::execFlip(flip fl) {
  queue<mask> q; const mask f=fl.f, l=fl.l, v=fl.v, u=f|l|v;
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
      SC[x]= (countBits(f&~x)==1)? ((f&x)|l|v): u;
      if(countBits(x)==dim) q.push(x);
      if(countBits(x)==dim-1) {
        if(in(x,base2)) adyBase2.insert(x);
        ++options[SC[x]];
    } }
    
    // Stuff that remain the same.
    else { 
      if(countBits(x)==dim-1) if(--options[SC[x]]==0) options.erase(SC[x]);
      SC[x]&=~u; SC[x]|= ((countBits(f&~x)==1)? ((f&x)|l|v): u);
      if(countBits(x)==dim-1) ++options[SC[x]];
    }

    /*
    if(countBits(x)==dim-1 && countBits(SC[x])==dim && !(in(x,base1)||in(x,base2))) {
      cout<<"Tracing down the lack of cheese"<<endl;
      printMask(x);
      printMask(old);
      printMask(older);
      if(SC.find(x)==SC.end()) cout<<"SC[x] not found"<<endl;
      printMask(SC[x]);
      printMask(f);
      printMask(l);
      printMask(v);
      printMask(u);
    }*/

  } while(nextSubset(u,x), x!=u);

  base1=SC[0]&LAYER1; base2=SC[0]&LAYER2; updateDists(q);

  if(!everythingIsOK()) {
    cout<<"Panic Attack at flip "<<numflips<<endl;
    printMask(f);
    printMask(l);
    printMask(v);
    assert(false);
  }
  numflips++;
}

// Makes a random move. Returns its (f,l,v).
flip prismatoid::execFlip() { flip fl=findFlip(); execFlip(fl); return fl;}

// Allow a flip with support u if:
// - It is an option (assumed)
// - There's room to add a new vertex (when required).
// - The ustar of f has exactly dim+1 vertices
// - The corresponding l is not in the complex.
// - It does not change the set of vertices (under changeBases==false)
// Returns the flip by reference.
bool prismatoid::checkFlip(mask u, flip& fl) {

  if(countBits(u)==dim) { // Add a vertex to the support when required.
    mask newv, LAYER;

    if     (countBits(u&LAYER2)==1) newv= firstElement(LAYER1 &~base1);
    else if(countBits(u&LAYER1)==1) newv= firstElement(LAYER2 &~base2);
    else cerr<<"Error 841: Not enough cheese in buffer."<<endl;
    
    if(newv==0) return false; u|=newv;
  }
  fl.f=u;
  for(mask x=firstElement(u); x!=0; nextElement(u,x))
    if(SC.find(u^x)!=SC.end()) fl.f&=u^x;
  fl.l=u^fl.f; 

  if      (countBits(u&base1)==1) fl.v=(u&base1), fl.f^=fl.v; 
  else if (countBits(u&base2)==1) fl.v=(u&base2), fl.f^=fl.v;
  else fl.v=0;

  // ustar(f) must have dim+1 vertices;
  if(countBits(SC[fl.f])!=dim+1) return false;

  // l must not be in SC.
  if(SC.find(fl.l)!=SC.end()) return false;

  // Am I adding/removing a vertex?
  if(!changeBases && (countBits(fl.l)==1 || countBits(fl.f)==1)) return false;

  return true;
}

////////////////////////////////////////////////////////////////////////////////
// S3: Costs and graph stuff
////////////////////////////////////////////////////////////////////////////////

// Inits graph and dists
void prismatoid::initGraph() {
  adyBase2=set<mask>(); queue<mask> q;

  for(auto &it:SC) {
    if(countBits(it.first)==dim-1) {
      if     ((LAYER1 & it.first)==0) adyBase2.insert(it.first);
      else if((LAYER2 & it.first)==0) q.push(it.second);
  } }
  dists=map<mask,il>(); updateDists(q);
}


// Stuff for the computation of the diameter and width.
inline void relaxPair(il& me, il& other) {
  if(other.first+1<me.first)       me.first = other.first+1,
                                   me.second= other.second;
  else if(other.first+1==me.first) me.second+=other.second;
}
void prismatoid::updateDists(queue<mask>& q) {
  while(!q.empty()) {
    mask f=q.front(), f2; q.pop();

    if(countBits(base1&f)==dim-1) {
      if(dists[f]==il(1,1)) continue;
      dists[f]=il(1,1);

      for(mask x= firstElement(f); x!=0; nextElement(f,x))
        if(SC.find(f^x)!=SC.end())
          if(countBits(f2=SC[f^x]^x)==dim) q.push(f2);
    }
    else {
      // if(dists[f]==ii(0,0)) dists[f]=ii(201,0); // needed?
      il aux=il(200,0);
      for(mask x= firstElement(f); x!=0; nextElement(f,x))
        if(SC.find(f^x)!=SC.end())
          if(countBits(f2=SC[f^x]^x)==dim && dists.find(f2)!=dists.end())
            relaxPair(aux, dists[f2]);

      if(aux!=dists[f]) {
        dists[f]=aux;
        for(mask x=firstElement(f); x!=0; nextElement(f,x))
          if(SC.find(f^x)!=SC.end())
            if(countBits(f2=SC[f^x]^x)==dim) q.push(f2);
} } } }

// Number of vertices, distance and width
pair<int, il> prismatoid::costs() {
  il aux(200,0);
  for(auto &it: adyBase2)
    relaxPair(aux, dists[SC[it]]);
  return make_pair(countBits(base1|base2), aux);
}

// f-vector and layers.
pair<vi, vi> prismatoid::statsForSantos() {
  vi fvector(dim+1), layers(dim+2);
  for(auto &it: SC) fvector[countBits(it.first)]++;
  for(auto &it: dists) layers[it.second.second]++;

  return make_pair(fvector, layers);
}

////////////////////////////////////////////////////////////////////////////////
// S4: Don't panic. Please don't panic.
////////////////////////////////////////////////////////////////////////////////

// Can I panic now?
// FIXME misterious cost (1,0) and (5,2)
bool prismatoid::everythingIsOK() {

  // 1: is SC a simplicial complex?
  /*
  for(auto& it: SC)
    for(mask x=firstElement(it.first); x!=0; nextElement(it.first,x))
      if(SC.find(it.first&~x)==SC.end()||!in(it.second,SC[it.first&~x])) { 
        cout<<"I find your lack of queso annoying"<<endl;
        cout<<"face and ustar"<<endl;
        printMask(it.first);
        printMask(it.second);
        cout<<"son and ustar"<<endl;
        printMask(it.first&~x);
        printMask(SC[it.first&~x]);
        return false;
      }
  /**/

  // 1.5: Pure simplicial complex
  //*
  for(auto &it: SC) 
    if(it.first==it.second && countBits(it.first)!=dim) {
      cout<<"sssh, no tears. Only "<<numflips<<" dreams now"<<endl;
      return false;
    }
  /**/

  // 2: Every ridge must be internal or in a base.
  //*
  for(auto& it: SC)
    if(countBits(it.first)==dim-1)
      if(it.second==it.first && !in(it.first,base1) && !in(it.first,base2)) {
        cout<<"Non-especified excuse at "<<numflips<<"! *flips table*"<<endl;
        return false;
      }
  /**/

  // 3: adybase2 is in fact adybase2
  // 4: options is options
  //*
  map<mask, int> otherOptions; set<mask> otherAdyBase2;
  for(auto& it: SC)
    if(countBits(it.first)==dim-1) {
      otherOptions[it.second]++;
      if(in(it.first, base2)) otherAdyBase2.insert(it.first);
    }
  if(options!=otherOptions) {
    cout<<"Number of cows backflipping: "<<numflips<<endl;
    return false;
  }
  if(adyBase2!=otherAdyBase2) {
    cout<<numflips<<" people talking about piranhas right now"<<endl;
    return false;
  }
  /**/

  // 5: dists
  //*
  
  /**/
  return true;
}
