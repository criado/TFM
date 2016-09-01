#include "prismatoid.hpp"

using namespace std;

int main() {
  prismatoid p(cin);
  assert(p.everythingIsOK());
  cout<<countBits(p.base1|p.base2)<<" "<<p.distBase2.first<<" "<<p.distBase2.second<<endl;
  vi fvector=p.statsForSantos().first;
  for(auto a:fvector) cout<<a<<" ";
  cout<<endl;
  
  return 0;
}
