# School quarantine model


## Highschool model

The highschool model is a discrete-time Agent-Based Model \[ABM\] that
includes the following features:

**Disease progression:**

- Susceptible individuals can aquire the virus from agents in the
  prodromal or rash state (as long as they are not isolated or
  quarantined).
- Exposed individuals develop the disease to transition to the prodromal
  state.
- Prodromal individuals develop the disease to transition to the rash
  state.
- Rash individuals are isolated.

**Quarantine process:**

- Individuals developing rash trigger quarantining process.
- Vaccinated individuals are skipped.
- Unvaccinated individuals may reject moving to quarantine.
- Individuals in quarantine who develop the disease trigger contact
  tracing as well.

**Vaccination:**

- Agents can be vaccinated at the beginning of the simulation.
- Vaccinated agents are less susceptible to the disease.
- Recovered agents are treated as immune.

### Parameters

The following is a raw list of the parameters included in the model:

    Warning in readLines(file, warn = readLines.warn): incomplete final line found
    on 'params.yaml'

    List of 18
     $ Population size          : int 1000
     $ 1/Rash period            : num 0.25
     $ Contact rate             : num 30
     $ Quarantine willingness   : num 0.5
     $ Incubation period        : num 5
     $ Max days in rash         : num 2
     $ Prodromal period         : num 4
     $ Quarantine days          : num 14
     $ Rash period              : num 4
     $ Transmission rate        : num 0.95
     $ Vaccination rate         : num 0.9
     $ Vax improved recovery    : num 0.5
     $ Vax reduced suscept.     : num 0.98
     $ initial number of exposed: num 1
     $ Seed                     : int 2231
     $ N days                   : int 60
     $ Replicates               : int 1000
     $ Threads                  : int 8

### Run

This model simulates the spread of measles in a highschool. The
highschool has 1000 students, and the simulation runs for 60 days with
one index case. The following is the output from the highschool model:

    Using file: params.yaml
    Starting multiple runs (1000) using 8 thread(s)
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
    Total elapsed t     : 4.00s (1000 runs)
    Last run speed      : 2.13 million agents x day / second
    Average run speed   : 13.75 million agents x day / second
    Rewiring            : off

    Global events:
     - Update model (runs daily)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - 1/Rash period             : 0.2500
     - Contact rate              : 10.0000
     - Incubation period         : 5.0000
     - Max days in rash          : 2.0000
     - N days                    : 60.0000
     - Population size           : 1000.0000
     - Prodromal period          : 4.0000
     - Quarantine days           : 14.0000
     - Quarantine willingness    : 0.9000
     - Rash period               : 4.0000
     - Replicates                : 1000.0000
     - Seed                      : 2231.0000
     - Threads                   : 8.0000
     - Transmission rate         : 0.9500
     - Vaccination rate          : 0.9000
     - Vax improved recovery     : 0.5000
     - Vax reduced suscept.      : 0.9800
     - initial number of exposed : 1.0000

    Distribution of the population at time 60:
      - (0) Susceptible             :  999 -> 994
      - (1) Exposed                 :    1 -> 0
      - (2) Prodromal               :    0 -> 0
      - (3) Rash                    :    0 -> 0
      - (4) Isolated                :    0 -> 0
      - (5) Quarantined Exposed     :    0 -> 0
      - (6) Quarantined Susceptible :    0 -> 0
      - (7) Quarantined Infectious  :    0 -> 0
      - (8) Recovered               :    0 -> 6

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.77  0.18  0.00  0.00  0.05  0.00  0.00  0.00
     - Prodromal                0.00  0.00  0.43  0.43  0.00  0.00  0.00  0.14  0.00
     - Rash                     0.00  0.00  0.00  0.50  0.50  0.00  0.00  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.76  0.00  0.00  0.00  0.24
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.75  0.00  0.25  0.00
     - Quarantined Susceptible  0.11  0.00  0.00  0.00  0.00  0.00  0.89  0.00  0.00
     - Quarantined Infectious   0.00  0.00  0.00  0.00  0.17  0.00  0.00  0.83  0.00
     - Recovered                0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00

## Flowchart

This diagram shows the possible transitions between the states in the
model:

``` mermaid
flowchart LR
    s0[Susceptible]
    s1[Exposed]
    s2[Prodromal]
    s3[Rash]
    s4[Isolated]
    s5[Quarantined Exposed]
    s6[Quarantined Susceptible]
    s7[Quarantined Infectious]
    s8[Recovered]
    s0 -->|0.000070| s1
    s0 -->|0.000017| s5
    s0 -->|0.003222| s6
    s1 -->|0.181818| s2
    s1 -->|0.045455| s5
    s2 -->|0.428571| s3
    s2 -->|0.142857| s7
    s3 -->|0.500000| s4
    s4 -->|0.235294| s8
    s5 -->|0.250000| s7
    s6 -->|0.106061| s0
    s7 -->|0.166667| s4
```

## Reading the output

    `stat_bin()` using `bins = 30`. Pick better value with `binwidth`.

![](README_files/figure-commonmark/reading-history-1.png)

Preparing the data for output

Some statistics about the contact tracing. Each detected case is moved
to the isolated state and triggers contact tracing. The following plot
shows the cumulative number of detected cases over time:

![](README_files/figure-commonmark/contact-tracing-1.png)
