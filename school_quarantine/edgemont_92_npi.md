# School quarantine model


## School quarantine model for edgemont_92_npi

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

| Parameter             | Value | Reference                                                           |
|:----------------------|------:|:--------------------------------------------------------------------|
| Contact rate          |  2.38 | Negative binomial. Calibrated using R0=15 as a reference.           |
| Incubation period     | 12.00 | Geometric + 1. Ref.: Jones and Baranowski (2019)                    |
| Max days in rash      |  2.00 | Fixed value.                                                        |
| Prodromal period      |  3.00 | Geometric + 1. Ref.: Jones and Baranowski (2019)                    |
| Quarantine days       | 21.00 | Utah Measles Disease Plan (“Measles Disease Plan” 2019).            |
| Rash period           |  4.00 | Geometric + 1. Ref.: “Measles Disease Plan” (2019).                 |
| Transmission rate     |  0.90 | Prob. of transmission fixed. Calibrated using R0=15 as a reference. |
| Vax improved recovery |  0.50 | Fixed value.                                                        |
| Vax efficacy          |  0.99 | Prob. efficacy fixed. Ref.: Liu et al. (2015).                      |
| R0                    | 15.00 | Theoretical R0.                                                     |

Other parameters can be found the corresponding parameters document
[here](edgemont_92_npi_params.yaml).

### Run

This model simulates the spread of measles in a highschool. The
highschool has students, and the simulation runs for days with one index
case. The following is the output from the highschool model:

    Using file: /tmp/Rtmpy1o1r7/file69f340cca11.yaml
    Starting multiple runs (2000) using 10 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : (none)
    Population size     : 634
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 60 (of 60)
    Number of viruses   : 1
    Last run elapsed t  : 0.00s
    Total elapsed t     : 1.00s (2000 runs)
    Last run speed      : 5.20 million agents x day / second
    Average run speed   : 49.67 million agents x day / second
    Rewiring            : off

    Global events:
     - Update model (runs daily)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - 1/Rash period          : 0.2500
     - Contact rate           : 2.3810
     - Incubation period      : 12.0000
     - Max days in rash       : 2.0000
     - Prodromal period       : 3.0000
     - Quarantine days        : 21.0000
     - Quarantine willingness : 1.0000
     - Transmission rate      : 0.9000
     - Vaccination rate       : 0.9200
     - Vax efficacy           : 0.9900
     - Vax improved recovery  : 0.5000

    Distribution of the population at time 60:
      - (0) Susceptible             : 633 -> 632
      - (1) Exposed                 :   1 -> 0
      - (2) Prodromal               :   0 -> 0
      - (3) Rash                    :   0 -> 1
      - (4) Isolated                :   0 -> 0
      - (5) Quarantined Exposed     :   0 -> 0
      - (6) Quarantined Susceptible :   0 -> 0
      - (7) Quarantined Infectious  :   0 -> 0
      - (8) Recovered               :   0 -> 1

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.91  0.04  0.00  0.00  0.04  0.00  0.00  0.00
     - Prodromal                0.00  0.00  0.78  0.22  0.00  0.00  0.00  0.00  0.00
     - Rash                     0.00  0.00  0.00  0.50  0.50  0.00  0.00  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.50  0.00  0.00  0.00  0.50
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.96  0.00  0.04  0.00
     - Quarantined Susceptible  0.05  0.00  0.00  0.00  0.00  0.00  0.95  0.00  0.00
     - Quarantined Infectious   0.00  0.00  1.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Recovered                0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00

## Flowchart

This diagram shows the possible transitions between the states in the
model:

``` mermaid
flowchart LR
    s0[Exposed]
    s1[Isolated]
    s2[Prodromal]
    s3[Quarantined Exposed]
    s4[Quarantined Infectious]
    s5[Quarantined Susceptible]
    s6[Rash]
    s7[Recovered]
    s8[Susceptible]
    s0 -->|0.079986| s2
    s0 -->|0.030884| s3
    s0 -->|0.002306| s4
    s1 -->|0.350498| s7
    s2 -->|0.014530| s4
    s2 -->|0.334436| s6
    s3 -->|0.081816| s4
    s4 -->|0.337662| s1
    s4 -->|0.042271| s2
    s5 -->|0.043159| s8
    s6 -->|0.424939| s1
    s6 -->|0.148902| s7
    s8 -->|0.000054| s0
    s8 -->|0.000005| s3
    s8 -->|0.001237| s5

```

## Outbreak size

Estimating the outbreak size:

|   Size | Probability    | Likely size (if \> Size) |
|-------:|:---------------|:-------------------------|
|  2.000 | 0.56           | \[2.00, 7.00\]           |
|  5.000 | 0.08           | \[5.00, 9.00\]           |
| 10.000 | \< 0.01        | \[10.00, 10.00\]         |
| 20.000 | \< 0.01        | \-                       |
|  2.000 | Median (50%\>) | \[ 3 , 8 \]              |
|  2.196 | Mean (average) | \[ 3 , 8 \]              |

Likely sizes of the outbreak based on 2000 simulations.

![](edgemont_92_npi_files/figure-commonmark/print-histogram-1.png)

Preparing the data for output

The following figure shows the cummulative number of new cases (detected
or not) over time. Cases can be identified with the following
transitions:

- Susceptible to Exposed.
- Susceptible to Quarantined Exposed.

![](edgemont_92_npi_files/figure-commonmark/contact-tracing-1.png)

### Reproductive number of the index case

![](edgemont_92_npi_files/figure-commonmark/reproductive-number-1.png)

    Mean R0:0.924

    Median R0:1

    95% CI R0:0,3.02499999999986

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
