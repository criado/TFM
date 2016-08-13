#ifndef COMPLEX        
#define COMPLEX        // Decaf ninja style. Bit ninjustu but with class.
#define N 15           //                                                      
                       // My humble interpretation of what SimplicialComplex.hpp
#include <map>         // should be.
#include <vector>      //
#include <algorithm>   // Author: Francisco "Paco" Criado 
#include <iostream>    // 
#include <fstream>     // I think my coding style is evolving into something 
#include <iomanip>     // more readable after this.                              
#include <queue>       
#include <ctime>       
#include <random>     
#include <cstdlib>
#include <set>
#include <string>

using namespace std;

typedef unsigned int mask;
typedef pair<int,int> ii;

classs Prismatoid { public:
  //////////////////////////////////////////////////////////////////////////////
  // Public stuff (that should be private)
  //////////////////////////////////////////////////////////////////////////////
  
  mask base1, base2;               // The actual vertices in each base.
  int n1, n2, dim;                 // n1,n2<=N defined above
  bool changeBases;                // Can we add/remove vertices?

  map<mask,mask> SC;               // Face and ustar of face
  map<mask, vector<mask>> graph;   // Face and list of adyacent faces
  vector<mask, ii> costs;          // Pair <distance, width> of each facet/base

  set<mask> options;               // Set of ustars of ridges 

  //////////////////////////////////////////////////////////////////////////////
  // Public methods
  //////////////////////////////////////////////////////////////////////////////
  
  // S1: Constructors and IO
  Prismatoid(int _dim);            // Crosspolytope
  Prismatoid(string file);         // Read prismatoid from file
  void write(string file);         // Write prismatoid to file

  // S2: Flippin' magic
  mask move();                     // These update the graph and the options set
  mask move(mask u);

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

  void addFacet(mask f);           // Sloooow
  void initOptions();              // Inits the option list 
  void initGraph();                // Inits the graph
  bool checkFlipability(mask u);   // Check if the l of the flip is in SC

  void diameter(queue<mask>& q);   // Update the graph with this queue of faces
  void relax(mask f);              // Update a vertex via BFS

  mask findMove();                 // Finds a valid move or crashes tryin'
}

#endif
