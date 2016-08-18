#include "prismatoid.hpp"
#include <fstream>
#include <functional>

int main() {
  unsigned seed=chrono::system_clock::now().time_since_epoch().count();
  rng generator(seed);
  uniform_real_distribution<double> dist(0.0,1.0);
  auto dice=bind(dist,generator);
  double totaltime=0.0; int totalflips=0;

  ofstream index,sol; ifstream file;

  flip fl; double record=28;

  for(int experiment=1; experiment<=100; experiment++) {
    double t=100000.0;
    double cost, oldCost;
    double avgCost=0.0, bestCost=1e30, numPrisms=0.0;
    int maxk=200000;

    #ifdef SANTOS
      file.open("./28prismatoid"); prismatoid p(file); file.close();
    #else
      prismatoid p(9);
    #endif

    for(int k=0; k<maxk; k++) {
      oldCost=p.cost();

      numPrisms++; avgCost+=oldCost; bestCost=min(bestCost,oldCost);
      if(((k+1)%1000)==0) {
        
        cout<<"Experiment "<<experiment
            <<" ("<<100*double(k+1)/maxk<<"%): "
            <<" temp= "<<t<<": "
            << oldCost <<" "<<avgCost/numPrisms<<" "<<bestCost
            <<" flip time: "<<totaltime/totalflips/1000.0<<"us."<<endl;
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

      t*=0.9999;
    }

    index.open("./outputs/index", ofstream::app);
    cout<<"Experiment "<<experiment
        << oldCost
        << endl;
    index.close();

    sol.open("./outputs/sol"+to_string(experiment));
    p.write(sol);
    sol.close();
  }

  return 0;
}
