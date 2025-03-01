# School quarantine model


## School quarantine model for davis_85

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

| Parameter             | Value | Reference                                                |
|:----------------------|------:|:---------------------------------------------------------|
| Contact rate          | 30.00 | Negative binomial. Calibrated using Texas data.          |
| Incubation period     | 12.00 | Geometric + 1. Ref.: Jones and Baranowski (2019)         |
| Max days in rash      |  2.00 | Fixed value.                                             |
| Prodromal period      |  3.00 | Geometric + 1. Ref.: Jones and Baranowski (2019)         |
| Quarantine days       | 21.00 | Utah Measles Disease Plan (“Measles Disease Plan” 2019). |
| Rash period           |  4.00 | Geometric + 1. Ref.: “Measles Disease Plan” (2019).      |
| Transmission rate     |  0.99 | Prob. of transmission fixed. Ref.: Liu et al. (2015).    |
| Vax improved recovery |  0.50 | Fixed value.                                             |
| Vax efficacy          |  0.99 | Prob. efficacy fixed. Ref.: Liu et al. (2015).           |

Other parameters can be found the corresponding parameters document
[here](davis_85_params.yaml).

### Run

This model simulates the spread of measles in a highschool. The
highschool has students, and the simulation runs for days with one index
case. The following is the output from the highschool model:

    Using file: /tmp/RtmppkGHOB/file36f1388dd39.yaml
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
    Total elapsed t     : 13.00s (1000 runs)
    Last run speed      : 1.40 million agents x day / second
    Average run speed   : 8.85 million agents x day / second
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
     - Quarantine days           : 21.0000
     - Quarantine willingness    : 1.0000
     - Rash period               : 4.0000
     - Replicates                : 1000.0000
     - Seed                      : 2231.0000
     - Threads                   : 10.0000
     - Transmission rate         : 0.9900
     - Vaccination rate          : 0.8500
     - Vax efficacy              : 0.9900
     - Vax improved recovery     : 0.5000
     - initial number of exposed : 1.0000

    Distribution of the population at time 60:
      - (0) Susceptible             : 1999 -> 1687
      - (1) Exposed                 :    1 -> 7
      - (2) Prodromal               :    0 -> 0
      - (3) Rash                    :    0 -> 2
      - (4) Isolated                :    0 -> 13
      - (5) Quarantined Exposed     :    0 -> 18
      - (6) Quarantined Susceptible :    0 -> 218
      - (7) Quarantined Infectious  :    0 -> 3
      - (8) Recovered               :    0 -> 52

    Transition Probabilities:
     - Susceptible              0.99  0.00  0.00  0.00  0.00  0.00  0.00  0.00  0.00
     - Exposed                  0.00  0.93  0.04  0.00  0.00  0.03  0.00  0.00  0.00
     - Prodromal                0.00  0.00  0.65  0.30  0.00  0.00  0.00  0.05  0.00
     - Rash                     0.00  0.00  0.00  0.50  0.50  0.00  0.00  0.00  0.00
     - Isolated                 0.00  0.00  0.00  0.00  0.81  0.00  0.00  0.00  0.19
     - Quarantined Exposed      0.00  0.00  0.00  0.00  0.00  0.93  0.00  0.07  0.00
     - Quarantined Susceptible  0.03  0.00  0.00  0.00  0.00  0.00  0.97  0.00  0.00
     - Quarantined Infectious   0.00  0.00  0.06  0.00  0.33  0.00  0.00  0.61  0.00
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
    s0 -->|0.000583| s1
    s0 -->|0.000226| s5
    s0 -->|0.004281| s6
    s1 -->|0.041669| s2
    s1 -->|0.028511| s5
    s1 -->|0.003981| s7
    s2 -->|0.300000| s3
    s2 -->|0.050000| s7
    s3 -->|0.500000| s4
    s4 -->|0.187488| s8
    s5 -->|0.070939| s7
    s6 -->|0.030303| s0
    s7 -->|0.062500| s2
    s7 -->|0.332127| s4

```

## Reading the output

![](davis_85_files/figure-commonmark/print-histogram-1.png)

Preparing the data for output

Some statistics about the contact tracing. Each detected case is moved
to the isolated state and triggers contact tracing. The following plot
shows the cumulative number of detected cases over time:

![](davis_85_files/figure-commonmark/contact-tracing-1.png)

### Reproductive number of the index case

![](davis_85_files/figure-commonmark/reproductive-number-1.png)

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
