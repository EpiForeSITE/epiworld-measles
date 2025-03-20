# Scenario modeling of measles using agent-based modeling
ForeSITE

[![ForeSITE
Group](https://github.com/EpiForeSITE/software/blob/e82ed88f75e0fe5c0a1a3b38c2b94509f122019c/docs/assets/foresite-software-badge.svg)](https://github.com/EpiForeSITE)

> [!CAUTION]
>
> This project is a work in progress. Use it at your own risk. **This
> model simulates a single school, so community transmission is not
> included**

> [!CAUTION]
>
> The code and model are still under development. We would love to hear
> your feedback and suggestions. Please open an issue in the [GitHub
> repository](https://github.com/EpiForeSITE/epiworld-measles) if you
> have any questions or suggestions.

This project aims to generate an ABM using
[epiworld](https://github.com/UofUEpiBio/epiworld) to do scenario
modeling of measles.

An ABM in which we pass the following inputs: - School size - If it is
elementary/mid/high school (three different models) - Seed cases -
Vaccination rate (which we would pull from DHHS) - Population
distribution (gender/age/etc) (we may need to do some lit review on
this). - Checklist on what interventions we would like to execute (TBD).

As outputs (with/without the interventions) - Number of cases as time
series. - Severity of the outbreak in terms of \$\$\$ (Rich team will
help). - The impact in how much time kids are taken out of school. -
Generate some baseline figures of these statistics.

# Data sources

- Immunization dashboard by Utah’s DHHS:
  https://avrpublic.dhhs.utah.gov/imms_dashboard/
- Measles disease plan by Utah’s DHHS:
  https://epi.utah.gov/wp-content/uploads/Measles-disease-plan.pdf

# Included models

- Contact tracing model ([contact_tracing](contact_tracing)): Contact
  tracing is performed and not all unvaccinated individuals are moved to
  quarantine.
- School quarantine model ([school_quarantine](school_quarantine)):
  There is no contact tracing and all unvaccinated individuals are moved
  to quarantine.

# $\mathcal{R_0}$ of measles

Measles $\mathcal{R}_0$ is estimated to be between 12 and 18, with a
mean of 15. For our case, we calibrate the model using the mean value of
15. Particularly, in our ABM, we have:

``` math
\begin{align*}
\mathcal{R}_0 & = \mathcal{R}_{0}^{prodromal} + \mathcal{R}_{0}^{rash} \\
& = C p_t \left(\mu_{prodromal} + \mu_{rash}\right) 
\end{align*}
```

Where $C$ is the number of contacts per day, $p_t$ is the probability of
transmission per contact, and $\mu_{prodromal}$ and $\mu_{rash}$ are the
average duration of the prodromal and rash periods, respectively.

To calculate the needed vaccination rate to achieve herd immunity, we
need to lower the transmission rate. To do so, we can reduce $p_t$ by
$(1 - v)$, where $v$ is the proportion of vaccinated individuals. We can
write:

``` math
\begin{align*}
1 & > C \times p_t (1 - v) \left(\mu_{prodromal} + \mu_{rash}\right) \\
1 & > \mathcal{R}_0 (1 - v) \\
1 - v & < \frac{1}{\mathcal{R}_0} \\
v & > 1 - \frac{1}{\mathcal{R}_0} \\
\end{align*}
```

An example

``` r
# Expected C for a transmission rate of .90, prodromal period of four
# days, and rash period of three days:
15 / .9 / (4 + 3)
## [1] 2.380952

# Vaccination rate needed to achieve herd immunity
1 - 1 / 15
## [1] 0.9333333

# R0 for a community with 88% vaccination rate
(Rt <- 15 * (1 - .88))
## [1] 1.8
```

Using the above equations, we can compute the expected number of cases
for different vaccination rates:

![](README_files/figure-commonmark/herd_immunity-1.png)
