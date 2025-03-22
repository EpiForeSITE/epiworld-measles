# School quarantine model


## School quarantine model for new_bridge_88_npi

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
[here](new_bridge_88_npi_params.yaml).

### Run

This model simulates the spread of measles in a highschool. The
highschool has students, and the simulation runs for days with one index
case. The following is the output from the highschool model:

    Using file: /tmp/RtmpP5yFn1/file5912586bec17.yaml
    Starting multiple runs (2000) using 10 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.
    ________________________________________________________________________________
    ________________________________________________________________________________
    SIMULATION STUDY

    Name of the model   : (none)
    Population size     : 614
    Agents' data        : (none)
    Number of entities  : 0
    Days (duration)     : 60 (of 60)
    Number of viruses   : 1
    Last run elapsed t  : 2.00ms
    Total elapsed t     : 570.00ms (2000 runs)
    Last run speed      : 13.76 million agents x day / second
    Average run speed   : 129.09 million agents x day / second
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
     - Vaccination rate       : 0.8800
     - Vax efficacy           : 0.9900
     - Vax improved recovery  : 0.5000

    Distribution of the population at time 60:
      - ( 0) Susceptible             : 613 -> 539
      - ( 1) Exposed                 :   1 -> 0
      - ( 2) Prodromal               :   0 -> 0
      - ( 3) Rash                    :   0 -> 0
      - ( 4) Isolated                :   0 -> 0
      - ( 5) Quarantined Exposed     :   0 -> 0
      - ( 6) Quarantined Susceptible :   0 -> 72
      - ( 7) Quarantined Prodromal   :   0 -> 0
      - ( 8) Quarantined Recovered   :   0 -> 3
      - ( 9) Hospitalized            :   0 -> 0
      - (10) Recovered               :   0 -> 0

    Transition Probabilities:
     - Susceptible              1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.95  0.02  0.00  0.00  0.00  0.00  0.02  0.00  0.00  0.00
     - Prodromal                0.00  0.00  0.00  1.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Rash                     0.00  0.00  0.00  0.50  0.00  0.00  0.00  0.00  0.50  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00  0.00  0.00
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00  0.00  0.00  0.00
     - Quarantined Susceptible  0.00  0.00  0.00  0.00  0.00  0.00  1.00  0.00  0.00  0.00  0.00
     - Quarantined Prodromal    0.00  0.00  0.00  0.00  1.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Quarantined Recovered    0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00  1.00  0.00  0.00
     - Hospitalized                -     -     -     -     -     -     -     -     -     -     -
     - Recovered                   -     -     -     -     -     -     -     -     -     -     -

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
    s0 -->|0.080234| s3
    s0 -->|0.031835| s4
    s0 -->|0.003281| s5
    s1 -->|0.134082| s9
    s2 -->|0.154196| s1
    s2 -->|0.612937| s6
    s2 -->|0.000699| s8
    s2 -->|0.006993| s9
    s3 -->|0.009264| s2
    s3 -->|0.015270| s5
    s3 -->|0.329431| s8
    s4 -->|0.012916| s0
    s4 -->|0.001700| s3
    s4 -->|0.084920| s5
    s5 -->|0.648385| s2
    s5 -->|0.006762| s3
    s5 -->|0.010143| s8
    s6 -->|0.055618| s9
    s7 -->|0.043747| s10
    s8 -->|0.145059| s1
    s8 -->|0.113955| s2
    s8 -->|0.308560| s6
    s8 -->|0.326727| s9
    s10 -->|0.000068| s0
    s10 -->|0.000007| s4
    s10 -->|0.001673| s7

```

## Outbreak size

Estimating the outbreak size:

|    Size | Probability    | Likely size (if \> Size) |
|--------:|:---------------|:-------------------------|
|  2.0000 | 0.60           | \[2.00, 10.00\]          |
|  5.0000 | 0.16           | \[5.00, 14.00\]          |
| 10.0000 | 0.02           | \[10.00, 17.90\]         |
| 20.0000 | \< 0.01        | \[20.00, 20.00\]         |
|  2.0000 | Median (50%\>) | \[3.00, 11.00\]          |
|  2.7285 | Mean (average) | \[3.00, 11.00\]          |

Likely sizes of the outbreak based on 2000 simulations.

![](new_bridge_88_npi_files/figure-commonmark/print-histogram-1.png)

Preparing the data for output

The following figure shows the cummulative number of new cases (detected
or not) over time. Cases can be identified with the following
transitions:

- Susceptible to Exposed.
- Susceptible to Quarantined Exposed.

![](new_bridge_88_npi_files/figure-commonmark/contact-tracing-1.png)

### Reproductive number of the index case

![](new_bridge_88_npi_files/figure-commonmark/reproductive-number-1.png)

Althougth the model was calibrated with an R0 of 15, adding vaccination,
a smaller population, and quarantine changes (lowers) the reproductive
number:

    Mean Rt:1.0725

    Median Rt:1

    95% CI Rt:0,4

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
