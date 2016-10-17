#ifndef MALE_CPP
#define MALE_CPP

#include <Rcpp.h>
using namespace Rcpp;

class Male {
public:
  int id;
  double mass, energy;
  double next_event;
  
  Male (int id_, double density) {
    id = id_;
    next_event = R::rexp(density);
  }
};

#endif
