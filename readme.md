# Introduction
Contests over mates can be very costly.

Alternative reproductive strategies (ART's) can develop as a way to minimise these costs.
This package implements an individual based model (sometimes called an agent based model),
that explores how and why these ART's emerge in a species x, which has non overlapping
generations.

## Model
### setup
The males mature before the females. All females mature at the same time. 

Starting at time `0`, we pull the time of an immature males maturation from
an exponential distribution with rate `maturation_rate`.

The longer males take to mature the more mass they have. 

Mature males then explore a patch and encounter nests.
For each male, we pull the time of the next nest encounter from the exponential distribution
with the rate `density`.


A nest is then selected. If it is guarded/occupied by a male then a contest over ownership of the nest will take place, if it is not occupied the searching male will then guard/occupy the nest.

Males that are occupying nests when the females mature get to mate, and pass on their
traits to the next generation of males.

Each occupied nest produces `males_per_nest` immature males in the next generation.
The new males inherit their fathers genes with a chance to add some gaussian noise.

### contests

TODO

### parameters
TODO

## Usage and installation
You can install this package with devtools.

```{r}
if(!require(devtools)) install.packages("devtools")
devtools::install_github("johnwilshire/contest")
```
# Implementation
This package was created using Rcpp.

## TODO
* Tests (Which? How? Where?)
* Generation
    * generation 2+ constructor
* Males
    * fighting
    * immature mortality ? `Why is this needed in the model again?` 
* logging
    * log deaths and energy expenditure
* finish DESCRIPTION file
* parameters from json function wrapper
* optimisations
    * for parameters that will be used a lot in a class save them to the class
     to avoid having to look them up from the parameters map

## Milestones 
* `one generation`
* `log winners`
* log stuff (deaths, energy ect)
* complete model