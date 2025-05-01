# Mixing example


In the mixing model, the contact rate is hold constant, so, although we
do have the mixing between groups at different rates, the average number
of contacts is kept constant. This facilitates calibration as the basic
reproductive number is not affected by the mixing matrix.

``` r
library(epiworldR)
library(data.table)
library(ggplot2)
```

``` r
# Creating populations
sizes <- c(300, 500, 5000)
school1 <- entity("School 1", sizes[1], as_proportion = FALSE)
school2 <- entity("School 2", sizes[2], as_proportion = FALSE)
rest    <- entity("18+", sizes[3], as_proportion = FALSE)

# Creating the row-stochastic mixing matrix
cmatrix <- c(
  c(0.8, 0.1, 0.1), # School 1
  c(0.1, 0.8, 0.1), # School 2
  c(0.05, 0.05, 0.9)  # Rest
) |> matrix(byrow = TRUE, nrow = 3)

N <- sum(sizes)
mixing_model <- ModelSEIRMixing(
  name = "Mixing example",
  n = N,
  prevalence = 2/N,
  contact_rate = 2.5,
  contact_matrix = cmatrix, # * 0 + 1/3,
  transmission_rate = 0.1,
  recovery_rate = 1/7,
  incubation_days = 14
)

# Adding the entities
mixing_model |>
  add_entity(school1) |>
  add_entity(school2) |>
  add_entity(rest)
```

We now create how we save the data

``` r
saver <- make_saver("total_hist", "reproductive", "transition")
```

``` r
set.seed(3312)
run_multiple(
  mixing_model, ndays = 60, nsims = 1000, saver = saver,
  nthreads = 10
)
```

    Starting multiple runs (1000) using 10 thread(s)
    _________________________________________________________________________
    _________________________________________________________________________
    ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| done.

``` r
dat <- run_multiple_get_results(mixing_model) |>
  lapply(data.table::data.table)
```

``` r
r0 <- dat$reproductive[source_exposure_date == 0 & source != -1, ]

R0_expected <- get_param(mixing_model, "Contact rate") * 
  get_param(mixing_model, "Prob. Transmission") /
  get_param(mixing_model, "Prob. Recovery")

sprintf(
  "Mean R0: %.2f [%.2f, %.2f] (expected: %.2f)",
  mean(r0$rt),
  quantile(r0$rt, 0.025),
  quantile(r0$rt, 0.975),
  R0_expected
)
```

    [1] "Mean R0: 1.76 [0.00, 7.00] (expected: 1.75)"

``` r
ggplot(r0, aes(x = rt)) +
  geom_histogram(bins = 50) +
  labs(
    title = "R0 distribution",
    x = "R0",
    y = "Count"
  ) +
  theme_minimal()
```

![](mixing_files/figure-commonmark/Computing%20R0-1.png)

``` mermaid
flowchart LR
    s0[Exposed]
    s1[Infected]
    s2[Recovered]
    s3[Susceptible]
    s0 -->|0.071849| s1
    s1 -->|0.143934| s2
    s3 -->|0.000034| s0
```
