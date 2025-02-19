# Measles models


## Highschool model

### Parameters

``` r
yaml::read_yaml("highschool-params.yaml") |>
    str()
```

    Warning in readLines(file, warn = readLines.warn): incomplete final line found
    on 'highschool-params.yaml'

    List of 19
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
     $ Replicates                  : int 500
     $ Threads                     : int 8

### Run

The following is the output from the highschool model:

    rm -f highschool-out/* && \
    ./highschool.o highschool-params.yaml
    Using file: highschool-params.yaml
    Starting multiple runs (500) using 8 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
     done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : (none)
    Population size     : 1000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 60 (of 60)
    Number of viruses   : 1
    Last run elapsed t  : 0.00s
    Total elapsed t     : 3.00s (500 runs)
    Last run speed      : 1.70 million agents x day / second
    Average run speed   : 9.84 million agents x day / second
    Rewiring            : off

    Global events:
     - Update model (runs daily)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - 1/Rash period                       : 0.2500
     - Contact rate                        : 10.0000
     - Contact tracing success rate        : 0.9000
     - How many days back to trace         : 14.0000
     - Incubation period                   : 5.0000
     - Max days in rash                    : 2.0000
     - N days                              : 60.0000
     - Population size                     : 500.0000
     - Prodromal period                    : 4.0000
     - Quarantine days                     : 14.0000
     - Rash period                         : 4.0000
     - Replicates                          : 500.0000
     - Seed                                : 2231.0000
     - Threads                             : 8.0000
     - Transmission rate                   : 0.9500
     - Vaccination rate                    : 0.9000
     - Vaccine reduction in recovery rate  : 0.5000
     - Vaccine reduction in susceptibility : 0.9800
     - Vax improved recovery               : 0.5000
     - Vax reduced suscept.                : 0.9800
     - initial number of exposed           : 1.0000

    Distribution of the population at time 60:
      - (0) Susceptible             :  999 -> 858
      - (1) Exposed                 :    1 -> 6
      - (2) Prodromal               :    0 -> 4
      - (3) Rash                    :    0 -> 1
      - (4) Isolated                :    0 -> 3
      - (5) Quarantined Exposed     :    0 -> 2
      - (6) Quarantined Susceptible :    0 -> 6
      - (7) Quarantined Infectious  :    0 -> 3
      - (8) Recovered               :    0 -> 117

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.70  0.23  0.00  0.00  0.05  0.00  0.02  0.00
     - Prodromal                0.00  0.00  0.73  0.24  0.00  0.00  0.00  0.02  0.00
     - Rash                     0.00  0.00  0.00  0.51  0.49  0.00  0.00  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.72  0.00  0.00  0.00  0.28
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.77  0.00  0.23  0.00
     - Quarantined Susceptible  0.08  0.00  0.00  0.00  0.00  0.00  0.92  0.00  0.00
     - Quarantined Infectious   0.00  0.00  0.05  0.00  0.20  0.00  0.00  0.75  0.00
     - Recovered                0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00

    Contact matrix (first 10 entires) (adjacency matrix)
    Each entry is the day of the contact between i and j, 
    with negative values indicating no contact 
    between the individuals.
      -1   16   34    9   -1   -1   10   -1   31   53 
      16   -1   39   -1   41   -1   36   -1   37   -1 
      34   39   -1   44   -1   39   14   18   11   13 
       9   -1   44   -1   52   -1   53    6   19   50 
      -1   41   -1   52   -1   28   20   -1   57   18 
      -1   -1   39   -1   28   -1   60   45   -1   53 
      10   36   14   53   20   60   -1   -1   -1   -1 
      -1   -1   18    6   -1   45   -1   -1   -1   53 
      31   37   11   19   57   -1   -1   -1   -1   36 
      53   -1   13   50   18   53   -1   53   36   -1 

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

``` r
library(data.table)
histories <- lapply(list.files(
    "highschool-out", pattern="total_hist", full.names = TRUE),
   \(fn) {
      cbind(fn = fn, data.table::fread(fn))
   }) |> rbindlist()


# Figuring out the outbreak size
exposed <- c("Exposed", "Predromal", "Rash", "Isolated", "Quarantined Exposed", "Quarantined Susceptible", "Recovered")
counts <- histories[(state %in% exposed) & (date == max(date))]
counts <- counts[, .(Total = sum(counts)), by = "fn"]

library(ggplot2)
ggplot(counts, aes(x = Total)) +
    geom_histogram()
```

    `stat_bin()` using `bins = 30`. Pick better value with `binwidth`.

![](README_files/figure-commonmark/reading-history-1.png)
