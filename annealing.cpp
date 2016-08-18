#include "prismatoid.hpp"
#include <fstream>
#include <functional>
//#define minvertices
#define maxwidth

double costsToDouble(pair<int,il> costs) {
#ifdef minvertices
  return double(costs.first);
#endif
#ifdef maxwidth
  if(costs.second.first>9) return -1e10;
  return costs.second.second;
#endif
}
bool feasible(pair<int,il> costs) {
#ifdef minvertices
  return costs.second.first>5;
#endif
#ifdef maxwidth
  return true;
#endif
}

int main() {
  unsigned seed=chrono::system_clock::now().time_since_epoch().count();
  rng generator(seed);
  uniform_real_distribution<double> dist(0.0,1.0);
  auto dice=bind(dist,generator);
  double totaltime=0.0; int totalflips=0;

  ofstream index,sol; ifstream file;

  flip fl; double record=28;

  for(int prueba=0; prueba<100; prueba++) {
#ifdef minvertices
    file.open("./28prismatoid"); prismatoid p(file); file.close();
#endif
#ifdef maxwidth
    prismatoid p(9);
#endif

    double t=100000.0;
    pair<int,il> costs, oldcosts;

    int maxk=200000;
    for(int k=0; k<maxk; k++) {
      oldcosts=p.costs();

      if(((k+1)%1000)==0) 
        cout<<"Prueba "<<prueba<<" ("<<100*double(k)/maxk<<"%): "
            <<" temp= "<<t<<": "<<oldcosts.first<<" ("
            <<oldcosts.second.first<<","
            <<oldcosts.second.second<<") average flip time: "
            <<totaltime/totalflips/1000.0<<"us."<<endl;

      auto start=chrono::steady_clock::now();
      while(true) {
        fl=p.execFlip(generator);
        costs=p.costs();

        swap(fl.f, fl.l);

        if(!feasible(costs)) {
          p.execFlip(fl);
          assert(p.costs()==oldcosts);
        }
        else break;
      }
      auto end=chrono::steady_clock::now();

      totaltime+=chrono::duration<double,nano>(end-start).count();
      totalflips++;

      double delta=costsToDouble(costs)-costsToDouble(oldcosts);

      if(delta>0.0 && dice()>exp(-delta/t)) {
        p.execFlip(fl);
        assert(oldcosts==p.costs());
      }

      t*=0.9999;
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
