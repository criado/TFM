#include "prismatoid.hpp"
#include <fstream>
#include <functional>
#include <iomanip>
#include <cmath>

double schedule(int k) {
  // "a ojo"-selected annealing schedule
  double t0=1000.0;
  return t0*pow(0.99997, k);
  /*
  return 1e-10;

  double t0=500.0;
  return t0/log(k+2);
  /**/
}

int main() {
  unsigned seed=chrono::system_clock::now().time_since_epoch().count();
  rng generator(seed);
  uniform_real_distribution<double> dist(0.0,1.0);
  auto dice=bind(dist,generator);
  double totaltime=0.0; int totalflips=0;

  flip fl; double record=28;

  for(int experiment=1; experiment>=1; experiment++) {
    int maxk=500000;
    double cost, oldCost, avgCost=0.0, bestCost=1e30, numPrisms=0.0;

    #ifndef PLAN_Z
      ifstream file("./28prismatoid"); prismatoid p(file); file.close();
    #else
      //prismatoid p(9);
      ifstream file("./outputs1/sol83"); prismatoid p(file); file.close();
    #endif

    for(int k=0; k<maxk; k++) {
      oldCost=p.cost();
      double t=schedule(k);

      numPrisms++; avgCost+=oldCost; bestCost=min(bestCost,oldCost);
      if(((k-1)%1000)==0) {
        
        cout<<"Experiment "<<experiment
            <<" ("<<setw(4)<<100*double(k-1)/maxk<<"%): "
            <<setw(5)<<" temp= "<<t<<": "
            << oldCost <<" "<<avgCost/numPrisms<<" "<<bestCost
            <<" flip time: "<<totaltime/totalflips/1000.0<<"us."<<endl;
        cout<<"  vertices:" <<countBits(p.base1|p.base2)
            <<" diameter:"<<p.distBase2.first<<endl;
        totaltime=0.0; totalflips=0;
        numPrisms=0.0; bestCost=1e30; avgCost=0.0;
      }

      auto start=chrono::steady_clock::now();
      while(true) {
        fl=p.execFlip(generator);
        cost=p.cost();

        swap(fl.f, fl.l);

        if(!p.feasible()) {
          p.execFlip(fl);
          assert(p.cost()==oldCost);
        }
        else break;
      }
      auto end=chrono::steady_clock::now();

      totaltime+=chrono::duration<double,nano>(end-start).count();
      totalflips++;

      double delta=cost-oldCost;

      if(delta>0.0 && dice()>exp(-delta/t)) {
        p.execFlip(fl);
        assert(oldCost==p.cost());
      }

      t=schedule(k);
    }

    ofstream index("./outputs/index", ofstream::app);
    index<<"Experiment "<<experiment
         <<" ("<< oldCost
         <<") Vertices: "<<countBits(p.base1|p.base2)
         <<" Diameter: "<<p.distBase2.first<<endl;
    index.close();

    ofstream sol("./outputs/sol"+to_string(experiment));
    p.write(sol);
    sol.close();
  }

  return 0;
}
