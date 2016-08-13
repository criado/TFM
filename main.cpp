// Truly a masterpiece to remember for generations of ninjas
// author: Francisco "Paco" Criado
#include "ninja.h"
#include <fstream>
#include <cmath>
#include <cstring>

int main() {
  uint oldcost, cost, f, l;
  pair<uint,uint> fl;
  double deltax,t0,t;
  times_called=1;

  srand(0);
  //cout<<rand()<<endl;
  ofstream index, sol;
  index.open("./index");

  for(int i=1;i<10; ++i) {
    memset(faces,0,maxf*sizeof(mydata));
    qfront=qback=0;
    crosspolytope();
    //print(cout);

    t0=t=1000.0;
               //  100000
    for(int k=0; k<10000; ++k){
      if(k%100==0) {
        cout<<i<<"/"<<k
          <<" temp: "<<t<<" coste: "
          <<uint(faces[base2].width)<<" ciclos: "
          <<cycles/times_called<<" tiempo: "
          <<((float) cycles)/times_called/CLOCKS_PER_SEC
          <<" n: "<<times_called<<endl;
        cycles=0; times_called=0;
      }
      oldcost=faces[base2].width;
      //cycles=cycles-clock(); 
      fl=randomFlip();
      //cycles=cycles+clock();
      f=fl.first; l=fl.second;
      //if(f==0520) {cout<<"postflip"<<endl;print(cout);}
      //cout<<"flipped "<<b(l)<<endl;
      cost=faces[base2].width;

      if(faces[base2].dist!=dim) {
        cout<<"win"<<endl;
        cout<<"printing to files"<<endl;
        index<<i<<" "<<uint(faces[base2].dist)<<","
          <<uint(faces[base2].width)<<endl;
        sol.open("./sol"+to_string(i));
        print(sol);
        sol.close();
        return 0;
      }
      double deltax=double(cost)-double(oldcost);
     
      if(deltax>=0 && (double(rand())/RAND_MAX)>exp(-deltax/t)) {
        //cout<<"back"<<endl;
        flip(l);
        if(faces[base2].width!=oldcost) {
          cerr<<"mal"<<endl;
          print(cout);
          cout<<b(f)<<" "<<b(l)<<endl;
          return 17;
        }
      }

      t*=0.999;
    }

    cout<<"printing to files"<<endl;
    index<<i<<" "<<uint(faces[base2].dist)<<","<<uint(faces[base2].width)<<endl;
    sol.open("./sol"+to_string(i));
    print(sol);
    sol.close();
  }

/*
  while(true) {
    char c; string dec;
    cin>>dec;

    flip(d(dec));
    diameter();
    print();
  }
  */

  return 0;
}
