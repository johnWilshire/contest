
#include <Rcpp.h>
#include <queue>
#include "male.cpp"
#include "nest.cpp"

using namespace Rcpp;

class Generation {
  std::priority_queue<Male, std::vector<Male>, Compare> males;
  std::vector<Nest> nests;
  
  int id;
  double time, maturation_time;
  double density;
  int num_nests, males_per_winner;
  std::map<std::string, double> parameters;
public:
  
  // init the first generation
  Generation (std::map<std::string, double> parameters_) {
    time = 0;
    id = 1; // this is the first generation
    parameters = parameters_;
    // initialise males
    for (double i = 0; i < parameters["num_nests"]; i++){
      males.push(Male(i, parameters));
      nests.push_back(Nest());
    }
    std::priority_queue<Male, std::vector<Male>, Compare> males2;
    
    while(!males.empty()) {
      Male m = males.top();
      Rcout << "id: " << m.id << " ttne: " << m.next_event;
      Rcout << " mass: " << m.mass << " energy: " <<  m.energy;
      Rcout << " a: " << m.alpha << " b: " << m.beta << std::endl;
      males.pop();
      males2.push(m);
    }
    
    males = males2;
  }
  
  // make a new generation from a previous one
  Generation (Generation * previous){
    
    
  }
  
  void start_generation () {
    Rcout << std::endl << "starting generation " << id <<  std::endl << std::endl;

    while(time < parameters["female_mat_time"] || males.empty()){
      Male male = males.top();
      double delta = male.next_event - time;
      males.pop();
      Rcout << "time: " << time << " delta: " << delta; 
      Rcout << "id: " << male.id << " energy: " << male.energy << std::endl;
      time = male.next_event;
      male.make_next_event(time);
      male.metabolise(delta);
      if (male.alive()) {
        males.push(male);
      } else {
        Rcout << "male: " << male.id << " has died." << std::endl;
      }
    }
  }
};