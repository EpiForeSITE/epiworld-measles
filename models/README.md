# Measles models


## Highschool model

The following is the output from the highschool model:

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
    Last run elapsed t  : 56.00ms
    Last run speed      : 1.05 million agents x day / second
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
     - How many days back to trace         : 14.0000
     - Incubation period                   : 5.0000
     - Max days in rash                    : 2.0000
     - Prodromal period                    : 4.0000
     - Quarantine days                     : 14.0000
     - Rash period                         : 4.0000
     - Transmission rate                   : 0.9500
     - Vaccination rate                    : 0.9000
     - Vaccine reduction in recovery rate  : 0.5000
     - Vaccine reduction in susceptibility : 0.9800

    Distribution of the population at time 60:
      - (0) Susceptible             :  999 -> 901
      - (1) Exposed                 :    1 -> 3
      - (2) Prodromal               :    0 -> 2
      - (3) Rash                    :    0 -> 1
      - (4) Isolated                :    0 -> 4
      - (5) Quarantined Exposed     :    0 -> 2
      - (6) Quarantined Susceptible :    0 -> 35
      - (7) Quarantined Infectious  :    0 -> 5
      - (8) Recovered               :    0 -> 47

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.68  0.19  0.00  0.00  0.11  0.00  0.02  0.00
     - Prodromal                0.00  0.00  0.70  0.26  0.00  0.00  0.00  0.04  0.00
     - Rash                     0.00  0.00  0.00  0.48  0.52  0.00  0.00  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.72  0.00  0.00  0.00  0.28
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.86  0.00  0.14  0.00
     - Quarantined Susceptible  0.08  0.00  0.00  0.00  0.00  0.00  0.92  0.00  0.00
     - Quarantined Infectious   0.00  0.00  0.05  0.00  0.22  0.00  0.00  0.73  0.00
     - Recovered                0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00

    Contact matrix (first 10 entires) (adjacency matrix)
    Each entry is the day of the contact between i and j, 
    with negative values indicating no contact 
    between the individuals.
      -1   -1   -1   44   -1   39   -1   60   -1   48 
      -1   -1   -1   -1   14   49   -1   -1   23   25 
      -1   -1   -1   -1   12   -1   21   36   13   -1 
      44   -1   -1   -1   55    7   -1   13   -1   -1 
      -1   14   12   55   -1   51   -1   -1   12   20 
      39   49   -1    7   51   -1   20   -1   53    6 
      -1   -1   21   -1   -1   20   -1   16   25   24 
      60   -1   36   13   -1   -1   16   -1   20   48 
      -1   23   13   -1   12   53   25   20   -1   11 
      48   25   -1   -1   20    6   24   48   11   -1 

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
