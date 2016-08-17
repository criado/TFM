#include "prismatoid.hpp"
#include <fstream>
#include <functional>

double costsToDouble(pair<int,il> costs) {
  return double(costs.first);
}
bool feasible(pair<int,il> costs) {
  return costs.second.first>5;
}

int main() {
  unsigned seed=chrono::system_clock::now().time_since_epoch().count();
  rng generator(seed);
  uniform_real_distribution<double> dist(0.0,1.0);
  auto dice=bind(dist,generator);
  double totaltime=0.0; int totalflips=0;

  ofstream index,sol; ifstream file;

  flip fl; double record=28;

  for(int prueba=0; prueba<10; prueba++) {
    file.open("./28prismatoid"); prismatoid p(file); file.close();

    double t=10000.0;
    pair<int,il> costs, oldcosts=p.costs();

    for(int k=0; k<10000; k++) {

      auto start=chrono::steady_clock::now();
      while(true) {
        fl=p.execFlip(generator); costs=p.costs();

        swap(fl.f, fl.l);

        if(!feasible(costs)) p.execFlip(fl);
        else break;
      }
      auto end=chrono::steady_clock::now();

      totaltime+=chrono::duration<double,nano>(end-start).count();
      totalflips++;

      if(((k+1)%1000)==0) 
        cout<<prueba<<": "<<costs.first<<" ("
            <<costs.second.first<<","
            <<costs.second.second<<") average flip time: "
            <<totaltime/totalflips/1000.0<<"ns."<<endl;

      double deltax=costsToDouble(costs)-costsToDouble(oldcosts);

      if(deltax>=0.0 && dice()>exp(-deltax/t)) p.execFlip(fl);
      
      oldcosts=costs; t*=0.999;
    }

    index.open("./index", ofstream::app);
    index<<prueba<<": "<<costs.first<<" ("
         <<costs.second.first<<","
         <<costs.second.second<<")"<<endl;
    index.close();

    sol.open("./sol"+to_string(prueba));
    p.write(sol);
    sol.close();
  }

  return 0;
}
