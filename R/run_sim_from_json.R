#' Runs a simulation using parameters defined in a Json file
#'
#' @param file The .json file defining the parameters.
#'
#' @return The result of calling run_simulation
#' @export
#'
#' @examples
#' run_sim_from_json('parameters.json')
run_sim_from_json <- function(file) {
  do.call(run_simulation, params = jsonlite::fromJSON(file))
}