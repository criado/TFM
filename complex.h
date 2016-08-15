#ifndef COMPLEX        
#define COMPLEX        // Decaf ninja style. Bit ninjustu but with class.
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

#define N 15
#define LAYER2 ((1<<N)-1)
#define LAYER1 (((1<<N)-1)<<N)

using namespace std;

typedef unsigned int mask;
typedef pair<int,int> ii;
typedef vector<int> vi;
typedef pair<mask,mask> mm;

classs Prismatoid { public:
  //////////////////////////////////////////////////////////////////////////////
  // Public stuff (that should be private)
  //////////////////////////////////////////////////////////////////////////////
  
  mask base1, base2;               // The actual vertices in each base.
  int dim;                         // A facet has dim vertices
  bool changeBases=false;          // Can we add/remove vertices?

  map<mask,mask> SC;               // Face and ustar of face
  map<mask, ii> dists;             // Pair <distance, width> of each facet
  set<mask> adyBase2;              // The set of the facets adyacent to base2

  map<mask,int> options;           // Set of ustars of ridges. That's it.
                                   // Frontier flips have dim vertices.
                                   // The int represents reference counting.

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  
  // S1: Constructors and IO
  Prismatoid(int _dim);            // Crosspolytope
  Prismatoid(istream& input);      // Reads prismatoid from file
  void write(ostream& output);     // Writes prismatoid to file

  // S2: Flippin' magic
  pair<mask,mask> move();          // These two update everything.
  void move(pair<mask,mask>);      // Input: f and l.
                                   //  won't care about flipability.

  // S3: Costs and graph stuff
  pair<int, ii> costs();           // Number of vertices, distance and width
  pair<vi,vi> statsForSantos();    // f-vector and layers

  // S4: Dont panic
  bool everythingIsOK();           // Is everything OK?

  private:
  //////////////////////////////////////////////////////////////////////////////
  // Private stuff
  //////////////////////////////////////////////////////////////////////////////
  
  default_random_engine generator; // RNG. The one thing that must be private.

  void cascadeFacets();            // If SC contains the facet list, this
                                   // finishes the construction.
                                   // Only call from a constructor.

  void initOptions();              // Inits the options list 
  void initGraph();                // Inits graph and dists
  void updateCosts(queue<mask>& q);// Updates the facets in q, cascading

  pair<mask,mask> findMove();             // Finds a move or crashes tryin'
  bool check(mask u, pair<mask,mask>& fl);// Checks the validity of u.
}

//XXX should I rename "move" to "flip"?

#endif
