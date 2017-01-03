# Introduction
Contests over mates can be very costly.

Alternative reproductive strategies (ART's) can develop as a way to minimise these costs.
This package implements an individual based model (sometimes called an agent based model),
that explores how and why these ART's emerge in a theoretical species, which has non overlapping
generations.

# Model
## Imature phase

The males mature before the females. All females mature at the same time. 

Starting at time `0`, we pull the time of an immature males maturation from
the exponential distribution with scale parameter `maturation_rate`, which is the average time at which a male will mature.

The longer males take to mature the more mass they have, this is influenced by
the parameters `growth_a` and `growth_b`.
Their mass increases according to the differential equation: 
```
dm/dt = growth_a * m ^ growth_b
```
This is solved with the initial condtion `m(0) = initial_mass`

When males mature they have an `energy` budget. The ammount of energy a male has
when it matures is given by `energy = mass * mass_to_energy`.

Males have a metabolic cost of `metabolism` per time unit.

Mature males then explore a patch and encounter nests.
For each male, we pull the time of the next nest 
encounter from the exponential distribution
with the scale parameter `encounter_delta`. Which is the average time between encounters. 

A nest is then selected. If it is guarded/occupied by a male then a contest over ownership of the nest will take place, if it is not occupied the searching male will then guard/occupy the nest.

Males that are occupying nests when the females mature get to mate, and pass on their
traits to the next generation of males.

Each occupied nest produces `males_per_nest` immature males in the next generation.
The new males inherit their fathers genes with a chance `mutation_rate` to add some gaussian noise with mean 0 and standard deviation `mutation_sd`.

### Contests

Males make a judgement on their opponent based on mass difference. 
Each male calculates a commitment value, according to a commitment function.
Their traits
`alpha` and `beta` deterimine how sensitive a male are to percieved difference in mass.
For example male A's commitment against male B is

```
commitment = exp(beta)*((A.mass/B.mass)*z)^alpha
```
Where `z` is a random number drawn from an f distribution with degrees of freedom `contest_noise`, `contest_noise`. To represent the two judgements that a male has to make, that about its own size and that of its opponents.
If `contest_noise` equals 0 then z = 1.

The male that has chosen to commit more to the fight will win.

The cost of the fight to each male is the commitment, this is deducted from 
a males energy.

### Todo:
* Add noise to the commitment function (maybe)


### Parameters

Parameter         | Meaning
------------------|-------------------------------------------------------
max_gens          | Maximum number of generations to run the simulation for. 
males_per_winner  | Number of male offspring per occupied nest.
num_nests         | The number of nests that males can fight over. Also the initial number of males.
encounter_delta   | Mean time between nest enounters for a male.
metabolism        | How fast males use energy just by searching or occupying.
female_mat_time   | The time at which females mature.
maturation_rate   | Location parameter for the maturation of males.
mutation_rate     | How frequently mutations to traits occur.
mutation_sd       | mutations are mean zero with this much noise.
mass_to_energy    | Mass to energy scaling factor.
growth_a          | Growth parameter a.
growth_b          | Growth parameter b.
initial_mass      | The inital mass of a male.
alpha_mean        | The mean used to initialiseation of the `alpha` contest trait.
alpha_sd          | The mean used to initialiseation of the `alpha` contest trait.
beta_sd           | The sd used to initialiseation of the `beta` contest trait.
beta_max          | The maximum value of the `beta` contest trait.
beta_mean         | The mean used to initialiseation of the `beta` contest trait.
log_every         | Log traits of winners ever this number of generations. 
verbose           | Whether or not to print detailed mesages to the console.
quiet             | Whether or not to print generation messages.

## Usage and installation
You can install this package with devtools.

```{r}
if(!require(devtools)) install.packages("devtools")
devtools::install_github("johnwilshire/contest")
```

You can then run the model like so:

```{R}
library(ggplot2)
library(dplyr)

winners <- contest::run_simulation(
    max_gens = 1000,
    males_per_winner = 20,
    num_nests = 500,
    encounter_delta = 1,
    metabolism = 1,
    female_mat_time = 10, 
    maturation_rate = 1,
    mutation_rate = 0.1,
    mutation_sd = 0.1,
    mass_to_energy = 10,
    growth_a = 0.5,
    growth_b = 0.1,
    initial_mass = 5,
    alpha_mean = 0,
    alpha_sd = 3,
    beta_sd = 3,
    beta_mean = 0,
    beta_max = 20,
    verbose = F,
    quiet = F)


winners %>% filter((generation %% 50) - 1 == 0) -> reduced_winners

# plots of traits through generations
ggplot(reduced_winners, aes(generation, alpha)) + geom_point()
ggplot(reduced_winners, aes(generation, beta)) + geom_point()
```

# Implementation
This package was created using Rcpp.

## TODO
* Tests (Which? How? Where?)
* Males
    * immature mortality ? `Why is this needed in the model again?` 
* logging
    * log deaths and energy expenditure
* update licence, (is GPL ok?)
