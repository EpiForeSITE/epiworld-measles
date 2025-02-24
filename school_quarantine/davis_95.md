# School quarantine model


## School quarantine model for davis_95

This model is a discrete-time Agent-Based Model \[ABM\] that includes
the following features:

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
    on 'davis_95_params.yaml'

    List of 18
     $ Population size          : int 2000
     $ 1/Rash period            : num 0.25
     $ Contact rate             : int 30
     $ Quarantine willingness   : num 1
     $ Incubation period        : num 12
     $ Max days in rash         : num 2
     $ Prodromal period         : num 3
     $ Quarantine days          : num 14
     $ Rash period              : num 4
     $ Transmission rate        : num 0.99
     $ Vaccination rate         : num 0.95
     $ Vax improved recovery    : num 0.5
     $ Vax reduced suscept.     : num 0.99
     $ initial number of exposed: num 1
     $ Seed                     : int 2231
     $ N days                   : int 60
     $ Replicates               : int 1000
     $ Threads                  : int 10

### Run

This model simulates the spread of measles in a highschool. The
highschool has students, and the simulation runs for days with one index
case. The following is the output from the highschool model:

    Using file: /tmp/RtmpylAlfI/file7a9160c78ba.yaml
    Starting multiple runs (1000) using 10 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
     done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : (none)
    Population size     : 2000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 60 (of 60)
    Number of viruses   : 1
    Last run elapsed t  : 0.00s
    Total elapsed t     : 16.00s (1000 runs)
    Last run speed      : 1.27 million agents x day / second
    Average run speed   : 7.41 million agents x day / second
    Rewiring            : off

    Global events:
     - Update model (runs daily)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - 1/Rash period             : 0.2500
     - Contact rate              : 30.0000
     - Incubation period         : 12.0000
     - Max days in rash          : 2.0000
     - N days                    : 60.0000
     - Population size           : 2000.0000
     - Prodromal period          : 3.0000
     - Quarantine days           : 14.0000
     - Quarantine willingness    : 1.0000
     - Rash period               : 4.0000
     - Replicates                : 1000.0000
     - Seed                      : 2231.0000
     - Threads                   : 10.0000
     - Transmission rate         : 0.9900
     - Vaccination rate          : 0.9500
     - Vax improved recovery     : 0.5000
     - Vax reduced suscept.      : 0.9900
     - initial number of exposed : 1.0000

    Distribution of the population at time 60:
      - (0) Susceptible             : 1999 -> 1889
      - (1) Exposed                 :    1 -> 4
      - (2) Prodromal               :    0 -> 0
      - (3) Rash                    :    0 -> 0
      - (4) Isolated                :    0 -> 3
      - (5) Quarantined Exposed     :    0 -> 5
      - (6) Quarantined Susceptible :    0 -> 79
      - (7) Quarantined Infectious  :    0 -> 0
      - (8) Recovered               :    0 -> 20

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.90  0.06  0.00  0.00  0.04  0.00  0.00  0.00
     - Prodromal                0.00  0.00  0.56  0.33  0.00  0.00  0.00  0.11  0.00
     - Rash                     0.00  0.00  0.00  0.53  0.47  0.00  0.00  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.64  0.00  0.00  0.00  0.36
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.94  0.00  0.06  0.00
     - Quarantined Susceptible  0.03  0.00  0.00  0.00  0.00  0.00  0.97  0.00  0.00
     - Quarantined Infectious   0.00  0.00  0.09  0.00  0.27  0.00  0.00  0.65  0.00
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
    s0 -->|0.000224| s1
    s0 -->|0.000043| s5
    s0 -->|0.001483| s6
    s1 -->|0.059977| s2
    s1 -->|0.043290| s5
    s2 -->|0.333333| s3
    s2 -->|0.111111| s7
    s3 -->|0.474359| s4
    s4 -->|0.356667| s8
    s5 -->|0.056787| s7
    s6 -->|0.031250| s0
    s7 -->|0.086207| s2
    s7 -->|0.266667| s4

```

## Reading the output

![](davis_95_files/figure-commonmark/print-histogram-1.png)

Preparing the data for output

Some statistics about the contact tracing. Each detected case is moved
to the isolated state and triggers contact tracing. The following plot
shows the cumulative number of detected cases over time:

![](davis_95_files/figure-commonmark/contact-tracing-1.png)
