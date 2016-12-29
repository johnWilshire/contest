#' Title Runs run_simulation in parallel over parameters 
#' Provides a wrapper to perform a line search of a parameter and returns the 
#' results of run_simulation with the parameters specified.
#' 
#' @param params Parameters for `run_simulation`
#' @param varied_parameter 
#' @param values values to run the parameter over
#' @param mc_opts Options for mclapply, setting mc.cores is a good idea.
#'
#' @return A list of the result of calling run_simulation with the varied parameter.
#' @export
#' 
#' @examples
#'  params <- list(max_gens = 100,
#'                 males_per_winner = 20,
#'                 num_nests = 50,
#'                 density = 1,
#'                 metabolism = 1,
#'                 female_mat_time = 10, 
#'                 maturation_rate = 1,
#'                 mutation_rate = 0.1,
#'                 mutation_sd = 0.1,
#'                 mass_to_energy = 10,
#'                 growth_a = 0.5,
#'                 growth_b = 0.1,
#'                 initial_mass = 5,
#'                 alpha_mean = 0,
#'                 alpha_sd = 3,
#'                 beta_sd = 3,
#'                 beta_max = 20,
#'                 beta_mean = 0,
#'                 verbose = F,
#'                 quiet = T)
#'  run_parallel(params, "density", c(0.25, 0.5, 1), mc.cores = 4) -> sim
#'  Reduce(rbind, sim) -> df
#'  plot(df$generation, df$alpha, col = df$density)
#'  
run_parallel <- function(params, 
                         varied_parameter, 
                         values,
                         ...){
  parallel::mclapply(
    values, 
    FUN = function(x) {
      params[[varied_parameter]] <- x
      df <- do.call(run_simulation, params)
      df[[varied_parameter]] <- x
      df
    },
    ...)
}