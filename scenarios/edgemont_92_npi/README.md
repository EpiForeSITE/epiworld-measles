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

``` r
library(epiworldR)

abm <- with(temp_params, {
  ModelMeaslesQuarantine(
    n = `Population size`,
    prevalence = 1,
    # contact_rate = `Contact rate`,
    transmission_rate = `Transmission rate`,
    incubation_period = `Incubation period`,
    prodromal_period = `Prodromal period`,
    rash_period = `Rash period`,
    days_undetected = `Days undetected`,
    quarantine_period = `Quarantine days`,
    vax_efficacy = `Vax efficacy`,
    vax_improved_recovery = `Vax improved recovery`,
    prop_vaccinated = `Vaccination rate`,
    quarantine_willingness = `Quarantine willingness`,
    isolation_period = `Isolation period`
  )
})


saver <- make_saver("total_hist", "transition", "reproductive", "transition")

set.seed(3312)
run_multiple(
  abm, ndays = temp_params$`N days`,
  nthreads = temp_params$Threads,
  nsims = temp_params$`Replicates`,
  saver = saver
  )
```

    Starting multiple runs (2000) using 10 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.

## Flowchart

This diagram shows the possible transitions between the states in the
model:

``` mermaid
flowchart LR
    s0[Exposed]
    s1[Hospitalized]
    s2[Isolated]
    s3[Isolated Recovered]
    s4[Prodromal]
    s5[Quarantined Exposed]
    s6[Quarantined Prodromal]
    s7[Quarantined Susceptible]
    s8[Rash]
    s9[Recovered]
    s10[Susceptible]
    s0 -->|0.079868| s4
    s0 -->|0.044959| s5
    s0 -->|0.003888| s6
    s1 -->|0.147628| s9
    s2 -->|0.210003| s1
    s2 -->|0.525410| s3
    s2 -->|0.006184| s8
    s2 -->|0.012638| s9
    s3 -->|0.375016| s9
    s4 -->|0.015574| s2
    s4 -->|0.019190| s6
    s4 -->|0.322240| s8
    s5 -->|0.013994| s0
    s5 -->|0.001090| s4
    s5 -->|0.082644| s6
    s6 -->|0.662385| s2
    s6 -->|0.005199| s4
    s7 -->|0.043739| s10
    s8 -->|0.195740| s1
    s8 -->|0.127789| s2
    s8 -->|0.283976| s3
    s8 -->|0.272819| s9
    s10 -->|0.000081| s0
    s10 -->|0.000002| s5
    s10 -->|0.001140| s7

```

## Outbreak size

Estimating the outbreak size:

|   Size | Probability    | Likely size (if \> Size) |
|-------:|:---------------|:-------------------------|
|  2.000 | 0.61           | \[2.00, 13.00\]          |
|  5.000 | 0.23           | \[5.00, 15.00\]          |
| 10.000 | 0.04           | \[10.00, 17.80\]         |
| 20.000 | \< 0.01        | \-                       |
|  2.000 | Median (50%\>) | \[3.00, 14.00\]          |
|  3.141 | Mean (average) | \[4.00, 14.00\]          |

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

    Mean Rt:1.13893053473263

    Median Rt:1

    95% CI Rt:0.5,3

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
