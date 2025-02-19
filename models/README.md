# Measles models


## Highschool model

### Parameters

``` r
yaml::read_yaml("highschool-params.yaml") |>
    str()
```

    List of 18
     $ Population size             : int 500
     $ 1/Rash period               : num 0.25
     $ Contact rate                : num 10
     $ Contact tracing success rate: num 0.9
     $ How many days back to trace : num 14
     $ Incubation period           : num 5
     $ Max days in rash            : num 2
     $ Prodromal period            : num 4
     $ Quarantine days             : num 14
     $ Rash period                 : num 4
     $ Transmission rate           : num 0.95
     $ Vaccination rate            : num 0.9
     $ Vax improved recovery       : num 0.5
     $ Vax reduced suscept.        : num 0.98
     $ initial number of exposed   : num 1
     $ Seed                        : int 2231
     $ N days                      : int 60
     $ Replicates                  : int 100

### Run

The following is the output from the highschool model:

This model simulates the spread of measles in a highschool. The
highschool has 1000 students, and the simulation runs for 60 days with
one index case. We can check that transitions between states looking at
the transition matrix:

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.68  0.19  0.00  0.00  0.11  0.00  0.02  0.00
     - Prodromal                0.00  0.00  0.70  0.26  0.00  0.00  0.00  0.04  0.00
     - Rash                     0.00  0.00  0.00  0.48  0.52  0.00  0.00  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.72  0.00  0.00  0.00  0.28
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.86  0.00  0.14  0.00
     - Quarantined Susceptible  0.08  0.00  0.00  0.00  0.00  0.00  0.92  0.00  0.00
     - Quarantined Infectious   0.00  0.00  0.05  0.00  0.22  0.00  0.00  0.73  0.00
     - Recovered  

From it, we see:

- Exposed can move to prodromal, quarantined exposed, or quarantined
  infectious.
- Prodromal moves to Rash or Quarantined Prodromal (which is
  infectious).
- Rash moves to Isolated (and triggers contact tracing).
- Isolated moves to Recovered (full immunity).
- Quarantined Exposed either moves to quarantined infectious or to
  isolated. It can moved to exposed, but since the quarantine period is
  14 days, we don’t see any case.
- Quarantined Infectious moves to prodromal (if exits quarantine) or to
  isolated (if rash detected), in which case contact tracing is
  triggered.
