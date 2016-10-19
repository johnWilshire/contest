#ifndef LOGGER
#define LOGGER

#include <Rcpp.h>
#include "male.cpp"
using namespace Rcpp;

class Logger {
  std::map<std::string, double> parameters;
  
  // for winners
  std::vector<int> winner_gen;
  std::vector<double> winner_alpha;
  std::vector<double> winner_beta;
  
  Logger (std::map<std::string, double> parameters_){
    parameters = parameters_;
  }
  
  // adds a winners traits to the log
  void add_winner(int gen, Male * m){
    winner_gen.push_back(gen);
    winner_alpha.push_back(m->alpha);
    winner_beta.push_back(m->beta);
  }
  
  // returns a data frame log of winners at which generation and their traits
  DataFrame get_winners () {
    return DataFrame::create(
      _["generation"] = winner_gen,
      _["alpha"] = winner_alpha,
      _["beta"] =  winner_beta
    );
  }
  
  // todo a function that returns parameters as a lsit for R maybe
};

#endif
