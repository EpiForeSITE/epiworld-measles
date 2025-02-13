# Measles models


## Highschool model

``` r
system2("make", "highschool.o", wait = TRUE)
system2("./highschool.o", stdout = TRUE) |> cat(sep="\n")
```

    _________________________________________________________________________
    |Running the model...
    Updating the model on day 1
    Number of symptomatic agents: 0
    |Updating the model on day 2
    Number of symptomatic agents: 0
    |Updating the model on day 3
    Number of symptomatic agents: 0
    |Updating the model on day 4
    Number of symptomatic agents: 0
    ||Updating the model on day 5
    Number of symptomatic agents: 0
    |Updating the model on day 6
    Number of symptomatic agents: 1
    |Updating the model on day 7
    Number of symptomatic agents: 1
    |Updating the model on day 8
    Number of symptomatic agents: 0
    |Updating the model on day 9
    Number of symptomatic agents: 0
    ||Updating the model on day 10
    Number of symptomatic agents: 0
    |Updating the model on day 11
    Number of symptomatic agents: 0
    |Updating the model on day 12
    Number of symptomatic agents: 1
    |Updating the model on day 13
    Number of symptomatic agents: 2
    ||Updating the model on day 14
    Number of symptomatic agents: 1
    |Updating the model on day 15
    Number of symptomatic agents: 1
    |Updating the model on day 16
    Number of symptomatic agents: 3
    |Updating the model on day 17
    Number of symptomatic agents: 5
    |Updating the model on day 18
    Number of symptomatic agents: 3
    ||Updating the model on day 19
    Number of symptomatic agents: 0
    |Updating the model on day 20
    Number of symptomatic agents: 6
    |Updating the model on day 21
    Number of symptomatic agents: 8
    |Updating the model on day 22
    Number of symptomatic agents: 5
    |Updating the model on day 23
    Number of symptomatic agents: 7
    ||Updating the model on day 24
    Number of symptomatic agents: 8
    |Updating the model on day 25
    Number of symptomatic agents: 8
    |Updating the model on day 26
    Number of symptomatic agents: 8
    |Updating the model on day 27
    Number of symptomatic agents: 12
    ||Updating the model on day 28
    Number of symptomatic agents: 13
    |Updating the model on day 29
    Number of symptomatic agents: 16
    |Updating the model on day 30
    Number of symptomatic agents: 18
    |Updating the model on day 31
    Number of symptomatic agents: 19
    |Updating the model on day 32
    Number of symptomatic agents: 26
    ||Updating the model on day 33
    Number of symptomatic agents: 34
    |Updating the model on day 34
    Number of symptomatic agents: 36
    |Updating the model on day 35
    Number of symptomatic agents: 31
    |Updating the model on day 36
    Number of symptomatic agents: 28
    ||Updating the model on day 37
    Number of symptomatic agents: 34
    |Updating the model on day 38
    Number of symptomatic agents: 41
    |Updating the model on day 39
    Number of symptomatic agents: 44
    |Updating the model on day 40
    Number of symptomatic agents: 50
    |Updating the model on day 41
    Number of symptomatic agents: 53
    ||Updating the model on day 42
    Number of symptomatic agents: 51
    |Updating the model on day 43
    Number of symptomatic agents: 46
    |Updating the model on day 44
    Number of symptomatic agents: 45
    |Updating the model on day 45
    Number of symptomatic agents: 45
    |Updating the model on day 46
    Number of symptomatic agents: 43
    ||Updating the model on day 47
    Number of symptomatic agents: 42
    |Updating the model on day 48
    Number of symptomatic agents: 44
    |Updating the model on day 49
    Number of symptomatic agents: 46
    |Updating the model on day 50
    Number of symptomatic agents: 49
    ||Updating the model on day 51
    Number of symptomatic agents: 45
    |Updating the model on day 52
    Number of symptomatic agents: 32
    |Updating the model on day 53
    Number of symptomatic agents: 25
    |Updating the model on day 54
    Number of symptomatic agents: 32
    |Updating the model on day 55
    Number of symptomatic agents: 44
    ||Updating the model on day 56
    Number of symptomatic agents: 36
    |Updating the model on day 57
    Number of symptomatic agents: 37
    |Updating the model on day 58
    Number of symptomatic agents: 36
    |Updating the model on day 59
    Number of symptomatic agents: 24
    || done.
    Updating the model on day 60
    Number of symptomatic agents: 26
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
    Last run elapsed t  : 13.00ms
    Last run speed      : 4.31 million agents x day / second
    Rewiring            : off

    Global events:
     - Update model (runs daily)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - Contact rate                        : 10.0000
     - Contact tracing success rate        : 0.9000
     - How many days back to trace         : 5.0000
     - Incubation period                   : 5.0000
     - Max days in rash                    : 2.0000
     - Prodromal period                    : 4.0000
     - Quarantine days unvax               : 14.0000
     - Quarantine days vax                 : 14.0000
     - Rash period                         : 4.0000
     - Transmission rate                   : 0.9500
     - Vaccine reduction in recovery rate  : 0.5000
     - Vaccine reduction in susceptibility : 0.9000

    Distribution of the population at time 60:
      - (0) Susceptible            :  999 -> 256
      - (1) Exposed                :    1 -> 49
      - (2) Prodromal              :    0 -> 53
      - (3) Rash                   :    0 -> 26
      - (4) Isolated               :    0 -> 25
      - (5) Quarantined Exposed    :    0 -> 0
      - (6) Quarantined Unexposed  :    0 -> 0
      - (7) Quarantined Infectious :    0 -> 0
      - (8) Recovered              :    0 -> 591

    Transition Probabilities:
     - Susceptible             0.98  0.02  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                 0.00  0.79  0.21  0.00  0.00  0.00  0.00  0.00  0.00
     - Prodromal               0.00  0.00  0.74  0.26  0.00  0.00  0.00  0.00  0.00
     - Rash                    0.00  0.00  0.00  0.52  0.48  0.00  0.00  0.00  0.00
     - Isolated                0.00  0.00  0.00  0.00  0.65  0.00  0.00  0.00  0.35
     - Quarantined Exposed        -     -     -     -     -     -     -     -     -
     - Quarantined Unexposed      -     -     -     -     -     -     -     -     -
     - Quarantined Infectious     -     -     -     -     -     -     -     -     -
     - Recovered               0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00
