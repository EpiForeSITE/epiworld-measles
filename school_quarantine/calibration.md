# School quarantine model


## School quarantine model for calibration

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

| Parameter             |  Value | Reference                                                |
|:----------------------|-------:|:---------------------------------------------------------|
| Contact rate          |   4.25 | Negative binomial. Calibrated using Texas data.          |
| Incubation period     |  12.00 | Geometric + 1. Ref.: Jones and Baranowski (2019)         |
| Max days in rash      | 200.00 | Fixed value.                                             |
| Prodromal period      |   3.00 | Geometric + 1. Ref.: Jones and Baranowski (2019)         |
| Quarantine days       |  21.00 | Utah Measles Disease Plan (“Measles Disease Plan” 2019). |
| Rash period           |   4.00 | Geometric + 1. Ref.: “Measles Disease Plan” (2019).      |
| Transmission rate     |   0.10 | Prob. of transmission fixed. Ref.: Liu et al. (2015).    |
| Vax improved recovery |   0.50 | Fixed value.                                             |
| Vax efficacy          |   0.99 | Prob. efficacy fixed. Ref.: Liu et al. (2015).           |

Other parameters can be found the corresponding parameters document
[here](calibration_params.yaml).

### Run

This model simulates the spread of measles in a highschool. The
highschool has students, and the simulation runs for days with one index
case. The following is the output from the highschool model:

    Using file: /tmp/Rtmpg7IjJo/file5fc1a9ddd0d.yaml
    Starting multiple runs (500) using 10 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
     done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : (none)
    Population size     : 40000
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 50 (of 50)
    Number of viruses   : 1
    Last run elapsed t  : 0.00s
    Total elapsed t     : 73.00s (500 runs)
    Last run speed      : 2.23 million agents x day / second
    Average run speed   : 13.65 million agents x day / second
    Rewiring            : off

    Global events:
     - Update model (runs daily)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - 1/Rash period             : 0.2500
     - Contact rate              : 4.2500
     - Incubation period         : 12.0000
     - Max days in rash          : 200.0000
     - N days                    : 50.0000
     - Population size           : 40000.0000
     - Prodromal period          : 3.0000
     - Quarantine days           : 21.0000
     - Quarantine willingness    : 1.0000
     - Rash period               : 4.0000
     - Replicates                : 500.0000
     - Seed                      : 2231.0000
     - Threads                   : 10.0000
     - Transmission rate         : 0.1000
     - Vaccination rate          : 0.0e+00
     - Vax efficacy              : 0.9900
     - Vax improved recovery     : 0.5000
     - initial number of exposed : 50.0000

    Distribution of the population at time 50:
      - (0) Susceptible             : 39950 -> 39073
      - (1) Exposed                 :    50 -> 381
      - (2) Prodromal               :     0 -> 83
      - (3) Rash                    :     0 -> 27
      - (4) Isolated                :     0 -> 0
      - (5) Quarantined Exposed     :     0 -> 0
      - (6) Quarantined Susceptible :     0 -> 0
      - (7) Quarantined Infectious  :     0 -> 0
      - (8) Recovered               :     0 -> 436

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.92  0.08  0.00  0.00  0.00  0.00  0.00  0.00
     - Prodromal                0.00  0.00  0.70  0.30  0.00  0.00  0.00  0.00  0.00
     - Rash                     0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00
     - Isolated                    -     -     -     -     -     -     -     -     -
     - Quarantined Exposed         -     -     -     -     -     -     -     -     -
     - Quarantined Susceptible     -     -     -     -     -     -     -     -     -
     - Quarantined Infectious      -     -     -     -     -     -     -     -     -
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
    s0 -->|0.000432| s1
    s1 -->|0.081142| s2
    s2 -->|0.302809| s3
    s3 -->|1.000000| s8

```

## Reading the output

![](calibration_files/figure-commonmark/print-histogram-1.png)

Preparing the data for output

Some statistics about the contact tracing. Each detected case is moved
to the isolated state and triggers contact tracing. The following plot
shows the cumulative number of detected cases over time:

![](calibration_files/figure-commonmark/contact-tracing-1.png)

### Reproductive number of the index case

         sim_num       Rt
           <int>    <num>
      1:       0 2.533333
      2:       1 2.323529
      3:       2 2.317073
      4:       3 2.400000
      5:       4 2.000000
     ---                 
    496:     495 2.384615
    497:     496 1.971429
    498:     497 1.933333
    499:     498 2.533333
    500:     499 2.705882

![](calibration_files/figure-commonmark/reproductive-number-1.png)

    Mean R0:2.2497554613629

    Median R0:2

    95% CI R0:1,6

# References

<div id="refs" class="references csl-bib-body hanging-indent"
entry-spacing="0">

<div id="ref-jones2019measles" class="csl-entry">

Jones, Trahern W, and Katherine Baranowski. 2019. “Measles and Mumps:
Old Diseases, New Outbreaks.”

</div>

<div id="ref-liuRoleVaccinationCoverage2015" class="csl-entry">

Liu, Fengchen, Wayne T A Enanoria, Jennifer Zipprich, Seth Blumberg,
Kathleen Harriman, Sarah F Ackley, William D Wheaton, Justine L
Allpress, and Travis C Porco. 2015. “The Role of Vaccination Coverage,
Individual Behaviors, and the Public Health Response in the Control of
Measles Epidemics: An Agent-Based Simulation for California.” *BMC
Public Health* 15 (1): 447. <https://doi.org/10.1186/s12889-015-1766-6>.

</div>

<div id="ref-MeaslesDiseasePlan2019" class="csl-entry">

“Measles Disease Plan.” 2019. Utah Department of Health and Human
Services.
<https://epi.utah.gov/wp-content/uploads/Measles-disease-plan.pdf>.

</div>

</div>
