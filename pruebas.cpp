#include "prismatoid.hpp"

using namespace std;

int main() {
  prismatoid p(cin);
  cout<<"built"<<endl;
  pair<int,il> c=p.costs();
  cout<<p.dim<<endl;

  cout<<c.first<<" "<<c.second.first<<" "<<c.second.second<<endl;
  cout<<"printing dists"<<endl;
  for(auto &it: p.dists) {
    printMask(it.first);
    cout<<"("<<it.second.first<<","<<it.second.second<<")"<<endl;
    cout<<"----"<<endl;
  }

  return 0;
  while(true) {
    /*
    for(auto &it: p.SC) {
      printMask(it.first); printMask(it.second);
      cout<<"----"<<endl;
    }*/
    cout<<"printing dists"<<endl;
    for(auto &it: p.dists) {
      printMask(it.first);
      cout<<"("<<it.second.first<<","<<it.second.second<<")"<<endl;
      cout<<"----"<<endl;
    }
    cout<<"printing options"<<endl;
    for(auto &it: p.options) {
      printMask(it.first);
    }

    cout<<"enter to continue"<<endl;
    char c=cin.get();

    flip fl=p.findFlip();
    //flip fl; p.checkFlip(53,fl);
    
    printMask(fl.f);
    printMask(fl.l);
    printMask(fl.v);


    p.execFlip(fl);

  
  }

  return 0;
}

