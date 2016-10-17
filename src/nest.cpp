
#include <Rcpp.h>
#include "male.cpp"
using namespace Rcpp;

class Nest {
public:
  Nest (){}
  Male * occupier;
};