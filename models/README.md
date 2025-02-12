# Measles models


## Highschool model

``` r
system2("make", "highschool.o", wait = TRUE)
system2("./highschool.o", stdout = TRUE) |> cat(sep="\n")
```

    _________________________________________________________________________
    |Running the model...
    |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
    | done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : (none)
    Population size     : 1000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 60 (of 60)
    Number of viruses   : 1
    Last run elapsed t  : 1.00ms
    Last run speed      : 30.24 million agents x day / second
    Rewiring            : off

    Global events:
     (none)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - Contact tracing depth               : 5.0000
     - Contact tracing success rate        : 0.9000
     - Incubation period                   : 5.0000
     - Prodromal period                    : 5.0000
     - Quarantine days unvax               : 14.0000
     - Quarantine days vax                 : 14.0000
     - Rash period                         : 5.0000
     - Transmission rate                   : 0.9500
     - Vaccine reduction in recovery rate  : 0.5000
     - Vaccine reduction in susceptibility : 0.9000

    Distribution of the population at time 60:
      - (0) Susceptible            :  990 -> 990
      - (1) Exposed                :   10 -> 0
      - (2) Infectious             :    0 -> 0
      - (3) Symptomatic            :    0 -> 0
      - (4) Quarantined Exposed    :    0 -> 0
      - (5) Quarantined Unexposed  :    0 -> 0
      - (6) Quarantined Infectious :    0 -> 0
      - (7) Recovered              :    0 -> 10

    Transition Probabilities:
     - Susceptible             1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                 0.00  0.37  0.00  0.00  0.00  0.00  0.00  0.63
     - Infectious                 -     -     -     -     -     -     -     -
     - Symptomatic                -     -     -     -     -     -     -     -
     - Quarantined Exposed        -     -     -     -     -     -     -     -
     - Quarantined Unexposed      -     -     -     -     -     -     -     -
     - Quarantined Infectious     -     -     -     -     -     -     -     -
     - Recovered               0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00
