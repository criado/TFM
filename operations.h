// Truly a masterpiece to remember for generations of ninjas
// My interpretation of topaz/src/SimplicialComplex.cc
// I am well aware that this is awful style.
// author: Francisco "Paco" Criado
#ifndef OPERATIONS
#define OPERATIONS
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <random>
#include <map>
#include <set>
#include <vector>
#include <ctime>
#define n 10
#define dim 8
//every simplex has $dim$ vertices.

using namespace std;

extern clock_t cycles; extern int times_called;
typedef unsigned int uint; typedef unsigned char uchar; typedef short int sint;
typedef vector<int> vi; typedef pair<int,int> ii;

/*
 * A bitmask for an option has this structure:
 * 1 bit representing the new vertex to add (most significant);
 * n bits of potential vertices in base1;
 * n bits of potential vertices in base2;
 */
extern uint base1, base2;
extern map<uint, uint> SC;
extern map<uint, ii> graph;
extern set<uint> options;

void crosspolytope(); //inits everything with a crosspolytope
uint findFlip(); //Gets a valid random flip
void flip(uint u); //flips or unflips. Updates distances.
pair<int, ii> costs(); //returns a pair with the number of vertices and width

void readPrismatoid(string path); //gets a prismatoid from file
void writePrismatoid(string path); //prints a prismatoid to file
pair<vi, vi> statisticsForSantos(); //returns f-vector and layers.

#endif
