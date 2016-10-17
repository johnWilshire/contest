# Introduction
Contests over mates can be very costly.

Alternative reproductive strategies (ART's) can develop as a way to minimise these costs.
This package implements an individual based model (sometimes called an agent based model),
that explores how and why these ART's emerge in a species x, which has non overlapping
generations.


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
* Males
* Nests
* logging
* finish DESCRIPTION file
* parameters from json function wrapper

## Milestones 
* one generation
* log winners
* log stuff
* complete model