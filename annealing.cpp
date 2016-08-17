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
  default_random_engine generator(908421);
  uniform_real_distribution<double> dist(0.0,1.0);
  auto dice=bind(dist,generator);

  double record=28;
  ofstream index("./index");

  for(int prueba=0; prueba<10; prueba++) {
    ifstream file("./28prismatoid");
    prismatoid p(file);
    file.close();
    double t=10000.0;
    pair<int,il> costs, oldcosts=p.costs();

    for(int k=0; k<100000; k++) {
      flip fl=p.execFlip(); costs=p.costs();
      if(k%1000==0)
        cout<<prueba<<": "<<costs.first<<" ("
                          <<costs.second.first<<","
                          <<costs.second.second<<")"<<endl;

      swap(fl.f, fl.l);

      if(!feasible(costs)) {
        p.execFlip(fl);
        continue;
      }

      double deltax=costsToDouble(costs)-costsToDouble(oldcosts);

      if(deltax>=0.0 && dice()>exp(-deltax/t))
        p.execFlip(fl);
      
      oldcosts=costs;
      t*=0.999;
    }

    index<<prueba<<": "<<costs.first<<" ("
                       <<costs.second.first<<","
                       <<costs.second.second<<")"<<endl;

    ofstream sol("./sol"+to_string(prueba));
    p.write(sol);
    sol.close();
  }

  return 0;
}
