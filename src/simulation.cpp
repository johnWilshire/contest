
#include <Rcpp.h>
#include "generation.cpp"
#include "logger.cpp"

using namespace Rcpp;

class Simulation {
public:
  int current_gen, max_gens;
  double density, maturation_time;
  
  Logger logger;
  
  std::map<std::string, double> parameters;
  
  int num_nests, males_per_winner;
  
  Simulation (std::map<std::string, double> parameters_) : logger(parameters_) {
    parameters = parameters_;
    current_gen = 1;
  }
  
  void start () {
    Generation gen (parameters, &logger);
    int i = 1;
    if(!parameters["quiet"])
      Rcout << "running generation " << i << "\n";
    gen.run_generation();
    do {
      i++;
      if(!parameters["quiet"])
        Rcout << "running generation " << i << "\n";
      Generation current (&gen); // create a new generation with males 
      current.run_generation();
      gen = current;
    } while (i < parameters["max_gens"] && !gen.is_extinct());
    
  }
};

// [[Rcpp::export]]
DataFrame run_simulation(double max_gens, 
                    double males_per_winner,
                    double num_nests,
                    double female_mat_time,
                    double encounter_delta,
                    double metabolism,
                    double maturation_rate,
                    double mutation_rate,
                    double mutation_sd,
                    double mass_to_energy,
                    double growth_a,
                    double growth_b,
                    double initial_mass,
                    double alpha_mean,
                    double alpha_sd,
                    double beta_mean,
                    double beta_sd,
                    double beta_max,
                    bool verbose,
                    bool quiet = false,
                    int log_every = 10) {

  std::map<std::string, double> parameters;
  
  // setup the parameter map (this saves us from having to pass around heaps of arguments)
  parameters["max_gens"] = max_gens; 
  parameters["males_per_winner"] = males_per_winner; 
  parameters["num_nests"] = num_nests; 
  if(encounter_delta <= 0) Rcpp::stop("encounter_delta cannot be less than equal to zero!");
  parameters["density"] = encounter_delta;
  
  parameters["metabolism"] = metabolism;
  parameters["maturation_rate"] = maturation_rate;
  parameters["female_mat_time"] = female_mat_time;
  parameters["mutation_rate"] = mutation_rate;
  parameters["mutation_sd"] = mutation_sd;
  parameters["mass_to_energy"] = mass_to_energy;
  parameters["growth_a"] = growth_a;
  parameters["growth_b"] = growth_b;
  parameters["initial_mass"] = initial_mass;
  parameters["alpha_mean"] = alpha_mean;
  parameters["alpha_sd"] = alpha_sd;
  parameters["beta_mean"] = beta_mean;
  parameters["beta_sd"] = beta_sd;
  parameters["beta_max"] = beta_max;
  
  parameters["verbose"] = verbose ? 1 : 0;
  parameters["quiet"] = quiet ? 1 : 0;
  parameters["log_every"] = log_every;
  
  Simulation sim(parameters);
  // start the simulation
  sim.start();
  
  return sim.logger.get_winners();
}

