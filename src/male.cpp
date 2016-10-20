#ifndef MALE_CPP
#define MALE_CPP

#include <Rcpp.h>

using namespace Rcpp;

class Male {
public:
  int id;
  double mass, energy;
  double next_event; 
  double last_event; 
  double density; 
  double metabolism;
  double alpha, beta; // for contests
  
  // first generation constructor
  Male (int id_, std::map<std::string, double> parameters) {
    id = id_;
    density = parameters["density"];
    metabolism = parameters["metabolism"];
    
    double maturation = R::rexp(parameters["maturation_rate"]);
    last_event = maturation;
    make_next_event(maturation);
    grow(parameters["growth_a"],
         parameters["growth_b"],
         parameters["initial_mass"],
         maturation);
    
    energy = mass * parameters["mass_to_energy"];
    
    // traits
      // init traits from scratch
    alpha = R::rnorm(parameters["alpha_mean"], parameters["alpha_sd"]);
    beta = R::rnorm(parameters["beta_mean"], parameters["beta_sd"]);
  }
  
  Male (int id_, std::map<std::string, double> parameters, Male dad) {
    id = id_;
    density = parameters["density"];
    metabolism = parameters["metabolism"];
    
    double maturation = R::rexp(parameters["maturation_rate"]);
    last_event = maturation;
    make_next_event(maturation);
    grow(parameters["growth_a"],
         parameters["growth_b"],
                   parameters["initial_mass"],
                             maturation);
    
    energy = mass * parameters["mass_to_energy"];
    // inherit traits and mutate
    alpha = dad.alpha;
    beta = dad.beta;
    
    // mutate!
    if(R::runif(0,1) < parameters["mutation_rate"])
      alpha += R::rnorm(0, parameters["mutation_sd"]);
    
    if(R::runif(0,1) < parameters["mutation_rate"])
      beta += R::rnorm(0, parameters["mutation_sd"]);
  }
  
  void make_next_event (double time) {
    last_event = next_event;
    next_event = time + R::rexp(density);
  }
  
  void grow (double growth_a, double growth_b, 
              double initial_mass, double maturation) {
    mass = std::pow(initial_mass, 1 - growth_b); 
    mass += maturation * growth_a * ( 1.0 - growth_b);
    mass = std::pow(mass, 1.0 / (1.0 - growth_b));
  }
  
  bool alive () {
    return energy > 0;
  }
  
  void metabolise (double delta) {
    energy = energy - delta*metabolism;
  }
  
  void print () {
    Rcout << "id: "  << id << " energy: " << energy << std::endl;
  }
  
  // returns true if the occupier is the winner:
  // manages the metabolic costs of fighting from both parties
  bool fight(Male attacker){
    
    bool winner = false;
    double defender_commit = commitment(attacker.mass);
    double attacker_commit = attacker.commitment(mass);
    
    // the defender wins
    if (defender_commit >= attacker_commit){
      winner = true;
    }
    
    double commitment_delta = std::fabs(defender_commit - attacker_commit);
    // no probability of upset yet

    // get the cost of the fight
    double cost = std::min(energy, attacker.energy);
    cost = std::min(cost, commitment_delta);
    
    attacker.energy -= cost;
             energy -= cost;
    
    // todo log contest energy if needed
    
    // if the defender has died in te contest
    if(winner && !alive()){
      winner = false;
    }
    
    return winner;
  }
  
  // for sorting the vector
  bool operator < (const Male& m) {
    return next_event < m.next_event;
  }
  
  // returns the commitment of a male for the fight
  double commitment (double opponent_mass){
    return std::exp(beta) * std::pow((mass / opponent_mass), alpha);
  }

};

// for my priority_queue
class Compare
{
public:
  bool operator () (Male m1, Male m2) {
    return m1.next_event > m2.next_event;
  }
};

#endif
