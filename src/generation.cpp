
#include <Rcpp.h>
#include "male.cpp"
#include "nest.cpp"

using namespace Rcpp;

class Generation {
  std::vector<Male> males;
  std::vector<Nest> nests;
  
  int id;
  double time;
  double density;
  int num_nests, males_per_winner;
public:
  
  // init the first generation
  Generation (double density_,
              int num_nests_,
              int males_per_winner_) {
    time = 0;
    id = 1; // this is the first generation
    density = density_;
    males_per_winner = males_per_winner_;
    num_nests = num_nests_;
    
    // initialise males
    for (int i = 0; i < num_nests; i++){
      males.push_back(Male(i, density));
    }
    
    for (std::vector<Male>::iterator it = males.begin(); it != males.end(); ++it){
      Rcout << "id: " << it->id << " ttne: " << it->next_event <<std::endl;
    }
  }
  
  // make a new generation from a previous one
  Generation (Generation * previous){
    
    
  }
};