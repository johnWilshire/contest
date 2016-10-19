# Introduction
Contests over mates can be very costly.

Alternative reproductive strategies (ART's) can develop as a way to minimise these costs.
This package implements an individual based model (sometimes called an agent based model),
that explores how and why these ART's emerge in a species x, which has non overlapping
generations.

## Model
### setup
The males mature before the females. All females mature at the same time. 

Starting at time 0, we pull the time of male maturation from an exponential distribution with rate `maturation_rate`.

The longer males take to mature the more mass they have. 

Males then explore a patch and encounter nests.
For each male, we pull the time of the next nest encounter from the exponential distribution
with the rate `density`

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
    * make males a heap/priority queue
    * main loop
* Males
* Nests
* logging
    * log deaths and energy expenditure
* finish DESCRIPTION file
* parameters from json function wrapper
* optimisations
    * for parameters that will be used a lot in a class save them to the class

## Milestones 
* one generation
* log winners
* log stuff
* complete model