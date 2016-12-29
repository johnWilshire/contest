
#include <Rcpp.h>
#include <queue>
#include "male.cpp"
#include "logger.cpp"

using namespace Rcpp;
class Generation {
public:
  std::priority_queue<Male, std::vector<Male>, Compare> males;
  // instead of having a list of nests we will just have a list of occupying males
  // and when we pull an index out if the index is bigger than the list of occupiers
  // we the male will occupy the new list.
  std::vector<Male> occupiers;
  std::vector<Male> winners;
  
  int id;
  double time;
  bool log;
  bool verbose;
  Logger * logger;
  std::map<std::string, double> parameters;

  // init the first generation
  Generation (std::map<std::string, double> parameters_, Logger * logger_) {
    time = 0;
    id = 1; // this is the first generation
    parameters = parameters_;
    verbose = parameters["verbose"];
    
    logger = logger_;
    log = true;
    
    // initialise males
    for (double i = 0; i < parameters["num_nests"]; i++){
      males.push(Male(i, parameters));
    }
  }
  
  // make a new generation from a previous one
  Generation (Generation * previous){
    
    logger = previous->logger;
    time = 0;
    id = previous->id + 1;
    parameters = previous->parameters;
    verbose = parameters["verbose"];
    
    log = (int)id % (int)parameters["log_every"] == 0;
    
    int i = 0;
    for(std::vector<Male>::iterator it = previous->winners.begin();
          it != previous->winners.end(); ++it){
      for(double j = 0; j < parameters["males_per_winner"]; j++){
        males.push(Male(i, parameters, *it));
        i++;
      }
    }
  }
  // returns true if the nest is occupied
  bool is_occupied(int nest_index){
    return occupiers.size() > nest_index;
  }
  
  bool is_extinct(){
    return winners.size() == 0;
  }
  
  // the exploration phase
  void exploration(){
    while(time < parameters["female_mat_time"] && !males.empty()){
      Male male = males.top();
      double delta = male.next_event - time;
      males.pop();
      
      if (verbose){
        Rcout << "time: " << time << " delta: " << delta; 
        Rcout << " id: " << male.id << " energy: " << male.energy << std::endl;
      }
      // update time 
      time = male.next_event;
      male.make_next_event(time);
      male.metabolise(delta);
      
      if (male.alive()) {
        // select a nest to occupy 
        int nest_index = (int)std::floor(R::runif(0, parameters["num_nests"]));
        bool occupied = is_occupied(nest_index);
        
        // Rcout << "nest_index: " << nest_index << " occupiers size " << occupiers.size() << "\n";
        
        if (occupied){
          Male occupier = occupiers[nest_index];
          if(verbose){
            Rcout << "fight between attacker " << male.id << " and defender "<< occupier.id << "\n"; 
          }
          // metabolise the occupying male
          occupier.metabolise(time - male.last_event);
          occupier.make_next_event(time); // updates last event counter
          if(occupier.alive()){
            // a fight over the nest takes place
            // log fights
            // if the occupier has won
            if (occupier.fight(male)) {
              if (male.alive()){
                males.push(male); // add the male to the list of searching males
              }
            } else { // the occupier has lost
              if(occupier.alive()){
                males.push(occupier);
              }
              // the male will now inhabit the nest
              occupiers[nest_index] = male;
            }
            
          } else {
            // the attacker has won before the fight has started
            occupiers[nest_index] = male;
            // log male
          }
          
        } else {
          occupiers.push_back(male);
          if(verbose){
            Rcout << "nest is not occupied\n now male " << male.id << " is occupying" << std::endl;
          }
          
        }
        
        // return the winner and the loser
        // if the loser is still alive add them back into the male list'
        // the winner 
      } else if(verbose) {
        Rcout << "male: " << male.id << " has died." << std::endl;
      }
    }
  }
    
  void post_exploration(){
    // deduct energy costs for males in nests 
    // and add the surviving males to a winners list
    for(std::vector<Male>::iterator it = occupiers.begin();
        it != occupiers.end(); ++it){
      Male occupier = *it; 
      occupier.metabolise(parameters["female_mat_time"] - occupier.last_event);
      if(occupier.alive()){
        winners.push_back(occupier);
      } else {
        // incrememnt death counter
        if(verbose)
          Rcout << "after occupation, male: " << occupier.id << " has died." << std::endl;
      }
    }
  }
  
  void run_generation () {
    if(verbose)
      Rcout << std::endl << "starting generation " << id <<  std::endl << std::endl;
    exploration();
    
    if(verbose)
      Rcout << "females have matured" << std::endl<< std::endl;
    
    // clean up
    post_exploration();
    
    // print winners
    if(verbose)
      Rcout << "The winners for generation " << id << " are: \n";
    if(log){
      for(std::vector<Male>::iterator it = winners.begin();
          it != winners.end(); ++it){
        if(verbose)
          it->print();
        logger->log_winner(id, *it);
      } 
    }
  }
};