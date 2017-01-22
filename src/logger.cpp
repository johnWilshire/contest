#ifndef LOGGER
#define LOGGER

#include <Rcpp.h>
#include "male.cpp"
using namespace Rcpp;

class Logger {
public:
  std::map<std::string, double> parameters;
  
  // for winner_log
  std::vector<int> winner_gen;
  std::vector<double> winner_alpha;
  std::vector<double> winner_beta;
  std::vector<double> winner_mass;
  std::vector<int> winner_num_fights;
  
  
  Logger (std::map<std::string, double> parameters_){
    parameters = parameters_;
  }
  
  // adds a winners traits to the log
  void log_winner(int gen, Male m){
    winner_gen.push_back(gen);
    winner_alpha.push_back(m.alpha);
    winner_beta.push_back(m.beta);
    winner_mass.push_back(m.mass);
    winner_num_fights.push_back(m.num_fights);
  }
  
  // returns a data frame log of winners at which generation and their traits
  DataFrame get_winners () {
    return DataFrame::create(
      _["generation"] = winner_gen,
      _["alpha"] = winner_alpha,
      _["beta"] =  winner_beta,
      _["mass"] =  winner_mass,
      _["num_fights"] =  winner_num_fights
    );
  }
  
  // todo a function that returns parameters as a lsit for R maybe
};

#endif
