#ifndef PRISMATOID
 PRISMATOID     // Decaf ninja style. Bit ninjustu but with class.
                       // My humble interpretation of what SimplicialComplex.hpp
#include <map>         // should be.
#include <vector>      //
#include <algorithm>   // Author: Francisco "Paco" Criado
#include <iostream>    //
#include <fstream>     // I think my coding style is evolving into something
#include <iomanip>     // more readable after this.
#include <queue>       // Brief reminder: A simplex has dim vertices.
#include <ctime>
#include <chrono>
#include <random>
#include <cstdlib>
#include <set>
#include <string>
#include <bitset>
#include <cassert>
#include <cmath>
#include <Eigen/Dense>
#include "prismatoid.hpp"

#define N 14
#define LAYER2 ((1<<N)-1)
#define LAYER1 (((1<<N)-1)<<N)

using namespace std;
using namespace Eigen;

const double eps=1e-7;

class movement { public:
  vector<vi> v;

  static movement getRandom(double radius);
}

class geometricPrismatoid: public prismatoid{ public:
  //////////////////////////////////////////////////////////////////////////////
  // Public stuff (that should be private)
  //////////////////////////////////////////////////////////////////////////////

  //override: reading constructor, write, cost,
  map<mask, VectorXd> coordinates;
  map<mask,int> orientations;

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////

  // S3: Costs and graph stuff
  double cost();                   // Cost of this prismatoid (various options).
  bool feasible();                 // Do we want this prismatoid?

  // S4: Dont panic
  bool everythingIsOK();           // Is everything OK?

  private:
  //////////////////////////////////////////////////////////////////////////////
  // Private stuff
  //////////////////////////////////////////////////////////////////////////////

  void cascadeFacets();            // Completes the construction from the facets

  void initOptions();              // Inits the options list
  void initGraph();                // Inits graph and dists
  void updateDists(queue<mask>& q);// Updates dists by cascading

  flip findFlip(rng& gen);         // Finds a flip or crashes tryin'
  bool checkFlip(mask u, flip& fl);// Checks the validity of u as option,
};                                 //  returns the flip fl (by reference)

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

inline void printMask(mask f) {
  for(int i=0; i<2*N; i++) if(((1<<i)&f)!=0) cout<<i<<" "; cout<<endl;
}
inline mask readMask() {
  string str; getline(cin, str); mask res=0;
  for(int i=0; i<str.size(); i++) res=2*res+((str[i]=='1')?1:0);
  return res;
}

inline bool in(mask a, mask b) {return !(a&(~b));}

#endif

