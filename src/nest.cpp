
#include <Rcpp.h>
#include "male.cpp"
using namespace Rcpp;

class Nest {
public:
  Male * occupier;
  bool occupied;
  
  Nest(){
    occupied = false;
  }
  
  void occupy(Male x){
    occupier = &x;
    occupied = true;
  }
  
  void eject(){
    occupied = false;
  }
  
  
  // returns the loser of the contest
  // remember to update the time of last event
  Male fight(Male attacker){
    return attacker;
  }
};