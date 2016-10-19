
#include <Rcpp.h>
#include <queue>
#include "male.cpp"
#include "nest.cpp"
#include "logger.cpp"

using namespace Rcpp;

class Generation {
  std::priority_queue<Male, std::vector<Male>, Compare> males;
  std::vector<Nest> nests;
  std::vector<Male> winners;
  
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
  }
  
  // make a new generation from a previous one
  Generation (Generation * previous){
    
    
  }
  
  void start_generation () {
    Rcout << std::endl << "starting generation " << id <<  std::endl << std::endl;
    // main loop
    while(time < parameters["female_mat_time"] && !males.empty()){
      Male male = males.top();
      double delta = male.next_event - time;
      males.pop();
      if (parameters["verbose"] == 1){
        Rcout << "time: " << time << " delta: " << delta; 
        Rcout << " id: " << male.id << " energy: " << male.energy << std::endl;
      }
      
      time = male.next_event;
      male.make_next_event(time);
      male.metabolise(delta);
      
      if (male.alive()) {
        // select a nest to occupy 
        double nest_index = std::floor(R::runif(0, parameters["num_nests"]));
        Nest nest = nests[nest_index];
        
        if (nest.occupied){
          // fight over the nest  
          males.push(male);
        } else {
          nest.occupy(male);
        }
        
        // return the winner and the loser
        // if the loser is still alive add them back into the male list'
        // the winner 
      } else if(parameters["verbose"] == 1) {
        Rcout << "male: " << male.id << " has died." << std::endl;
      }
    }
    
    // deduct energy costs for males in nests 
    // and add the surviving males to a winners list
    for(std::vector<Nest>::iterator it = nests.begin();
          it != nests.end(); ++it){
      Nest nest = *it; 
      Rcout << "nest is " << (nest.occupied ? "occupied " : "not occupied") << std::endl;
      if(nest.occupied) {
        Male occupier = *nest.occupier;
        occupier.metabolise(parameters["female_mat_time"] - occupier.last_event);
        if(occupier.alive()){
          winners.push_back(occupier);
        } else {
          // incrememnt death counter
        }
      }
    }
    // print winners
    for(std::vector<Male>::iterator it = winners.begin();
        it != winners.end(); ++it){
      it->print();
    }
      
  }
};