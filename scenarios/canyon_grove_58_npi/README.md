# School quarantine model


## School quarantine model for canyon_grove_58_npi

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

| Parameter | Value | Reference |
|:---|---:|:---|
| Contact rate | 2.38 | Negative binomial. Calibrated using R0=15 as a reference. |
| Incubation period | 12.00 | Geometric + 1. Ref.: Jones and Baranowski (2019) |
| Days undetected | 2.00 | Fixed value. |
| Prodromal period | 3.00 | Geometric + 1. Ref.: Jones and Baranowski (2019) |
| Quarantine days | 21.00 | Utah Measles Disease Plan (“Measles Disease Plan” 2019). |
| Rash period | 4.00 | Geometric + 1. Ref.: “Measles Disease Plan” (2019). |
| Transmission rate | 0.90 | Prob. of transmission fixed. Calibrated using R0=15 as a reference. |
| Vax improved recovery | 0.50 | Fixed value. |
| Vax efficacy | 0.99 | Prob. efficacy fixed. Ref.: Liu et al. (2015). |
| R0 | 15.00 | Theoretical R0. |

Other parameters can be found the corresponding parameters document
[here](params.yaml).

### Run

This model simulates the spread of measles in a highschool. The
highschool has students, and the simulation runs for days with one index
case. The following is the output from the highschool model:

    Using file: /tmp/RtmpFZ5cmB/file2ac2b0e9ecc.yaml
    Starting multiple runs (2000) using 10 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : (none)
    Population size     : 650
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 120 (of 120)
    Number of viruses   : 1
    Last run elapsed t  : 0.00s
    Total elapsed t     : 1.00s (2000 runs)
    Last run speed      : 15.58 million agents x day / second
    Average run speed   : 131.60 million agents x day / second
    Rewiring            : off

    Global events:
     - Update model (runs daily)

    Virus(es):
     - Measles

    Tool(s):
     - Vaccine

    Model parameters:
     - Contact rate           : 2.3810
     - Days undetected        : 2.0000
     - Hospitalization days   : 7.0000
     - Hospitalization rate   : 0.2000
     - Incubation period      : 12.0000
     - Prodromal period       : 3.0000
     - Quarantine days        : 21.0000
     - Quarantine willingness : 1.0000
     - Rash days              : 4.0000
     - Transmission rate      : 0.9000
     - Vaccination rate       : 0.5800
     - Vax efficacy           : 0.9900
     - Vax improved recovery  : 0.5000

    Distribution of the population at time 120:
      - ( 0) Susceptible             : 649 -> 636
      - ( 1) Exposed                 :   1 -> 0
      - ( 2) Prodromal               :   0 -> 0
      - ( 3) Rash                    :   0 -> 0
      - ( 4) Isolated                :   0 -> 0
      - ( 5) Quarantined Exposed     :   0 -> 0
      - ( 6) Quarantined Susceptible :   0 -> 0
      - ( 7) Quarantined Prodromal   :   0 -> 0
      - ( 8) Quarantined Recovered   :   0 -> 0
      - ( 9) Hospitalized            :   0 -> 0
      - (10) Recovered               :   0 -> 14

    Transition Probabilities:
     - Susceptible              0.99  0.00  0.00  0.00  0.00  0.00  0.01  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.83  0.10  0.00  0.00  0.08  0.00  0.00  0.00  0.00  0.00
     - Prodromal                0.00  0.00  0.45  0.36  0.00  0.00  0.00  0.18  0.00  0.00  0.00
     - Rash                     0.00  0.00  0.00  0.00  0.50  0.00  0.00  0.00  0.00  0.50  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.14  0.00  0.00  0.00  0.57  0.29  0.00
     - Quarantined Exposed      0.00  0.03  0.00  0.00  0.00  0.91  0.00  0.06  0.00  0.00  0.00
     - Quarantined Susceptible  0.05  0.00  0.00  0.00  0.00  0.00  0.95  0.00  0.00  0.00  0.00
     - Quarantined Prodromal    0.00  0.00  0.00  0.00  0.71  0.00  0.00  0.29  0.00  0.00  0.00
     - Quarantined Recovered    0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.93  0.00  0.07
     - Hospitalized             0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.68  0.32
     - Recovered                0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00

## Flowchart

This diagram shows the possible transitions between the states in the
model:

``` mermaid
flowchart LR
    s0[Exposed]
    s1[Hospitalized]
    s2[Isolated]
    s3[Prodromal]
    s4[Quarantined Exposed]
    s5[Quarantined Prodromal]
    s6[Quarantined Recovered]
    s7[Quarantined Susceptible]
    s8[Rash]
    s9[Recovered]
    s10[Susceptible]
    s0 -->|0.073129| s3
    s0 -->|0.121877| s4
    s0 -->|0.011004| s5
    s1 -->|0.140200| s9
    s2 -->|0.202714| s1
    s2 -->|0.541247| s6
    s2 -->|0.003401| s8
    s2 -->|0.009655| s9
    s3 -->|0.034369| s2
    s3 -->|0.069946| s5
    s3 -->|0.294734| s8
    s4 -->|0.015093| s0
    s4 -->|0.001321| s3
    s4 -->|0.082290| s5
    s5 -->|0.655306| s2
    s5 -->|0.005566| s3
    s5 -->|0.010417| s8
    s6 -->|0.069043| s9
    s7 -->|0.045457| s10
    s8 -->|0.199123| s1
    s8 -->|0.134172| s2
    s8 -->|0.278380| s6
    s8 -->|0.277457| s9
    s10 -->|0.000178| s0
    s10 -->|0.000046| s4
    s10 -->|0.007414| s7

```

## Outbreak size

Estimating the outbreak size:

|    Size | Probability    | Likely size (if \> Size) |
|--------:|:---------------|:-------------------------|
|  2.0000 | 0.92           | \[2.00, 51.00\]          |
|  5.0000 | 0.71           | \[5.00, 52.55\]          |
| 10.0000 | 0.52           | \[10.00, 55.00\]         |
| 20.0000 | 0.30           | \[20.00, 59.92\]         |
| 10.0000 | Median (50%\>) | \[11.00, 55.00\]         |
| 15.0635 | Mean (average) | \[16.00, 58.00\]         |

Likely sizes of the outbreak based on 2000 simulations.

![](README_files/figure-commonmark/print-histogram-1.png)

Preparing the data for output

The following figure shows the cummulative number of new cases (detected
or not) over time. Cases can be identified with the following
transitions:

- Susceptible to Exposed.
- Susceptible to Quarantined Exposed.

![](README_files/figure-commonmark/contact-tracing-1.png)

### Reproductive number of the index case

![](README_files/figure-commonmark/reproductive-number-1.png)

Althougth the model was calibrated with an R0 of 15, adding vaccination,
a smaller population, and quarantine changes (lowers) the reproductive
number:

    Mean Rt:3.649

    Median Rt:3

    95% CI Rt:0,11

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
