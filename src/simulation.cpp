
#include <Rcpp.h>
//#include "male.cpp"
#include "generation.cpp"

using namespace Rcpp;

class Simulation {
public:
  int current_gen, max_gens;
  double density;
  
  int num_nests, males_per_winner;
  
  Simulation(int max_gens_, 
             double density_,
             int num_nests_,
             int males_per_winner_){
    max_gens = max_gens_;
    current_gen = 1;
    
    num_nests = num_nests_;
    males_per_winner = males_per_winner_;
    
    density = density_;
  }
  
  void start(){
    Generation gen ( density,
                    num_nests,
                    males_per_winner);
  }
};

// [[Rcpp::export]]
void run_simulation(int max_gens, 
                    double density,
                    int num_nests,
                    int males_per_winner) {
  
  // call the constructor
  Simulation sim(max_gens, 
                density,
                num_nests,
                males_per_winner);
  Rcout << "made a gen" << std::endl;
  // start the simulation
  sim.start();
  
}

