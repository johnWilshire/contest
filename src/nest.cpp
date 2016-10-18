
#include <Rcpp.h>
#include "male.cpp"
using namespace Rcpp;

class Nest {
  Male * occupier;
public:
  
  bool Occupied () {
    return occupier == NULL;  
  }
};