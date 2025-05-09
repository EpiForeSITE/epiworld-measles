# Implementation details

This model implements a time-discrete Agent-Based Model [ABM] with perfect mixing. The model starts with the introduction of one or more infected individuals. Depending on the vaccination rate, the model simulates the spread of the disease through a population of agents. The model includes a quarantine process that is triggered when an agent is detected in the rash period. The model includes the following parameters:

- **Contact rate**: Contact rate  .
- **Transmission rate**: Transmission rate.
- **Vax efficacy**: Vaccine reduction in susceptibility.
- **Vax improved recovery**: Vaccine reduction in recovery rate.
- **Incubation period**: Incubation period.
- **Prodromal period**: Prodromal period.
- **Rash period**: Rash period.
- **Days undetected**: Days undetected.
- **Hospitalization rate**: Hospitalization rate.
- **Hospitalization days**: Hospitalization duration.
- **Vaccination rate**: Vaccination rate.
- **Quarantine days**: Quarantine days for vaccinated.
- **Quarantine willingness**: Quarantine success rate.

The model was implemented in the [epiworld C++ library](https://github.com/UofUEpiBio/epiworld) and the R wrapper, [epiworldR](https://github.com/EpiForeSITE/epiworldR).

## States

The model is fairly complex and it includes ten different states:

0. Susceptible
1. Exposed
2. Prodromal
3. Rash
4. Isolated
5. Quarantined Exposed
6. Quarantined Susceptible
7. Quarantined Prodromal
8. Quarantined Recovered
9. Hospitalized
10. Recovered

Throughout the model, agents can either stay in their current state or transition to a new state. The following flowchart shows the overall transition patterns between all states in the model:

```mermaid
flowchart LR
    Susceptible -->|Undetected|Exposed
    Susceptible -->|Quarantined|QuarantineSusceptible["Quarantined<br>Susceptible"]
    Susceptible -->|Quarantined|QuarantineExposed["Quarantined<br>Exposed"]

    Exposed -->|Undetected|Prodromal
    Exposed -->|Quarantined|QuarantineExposed["Quarantined<br>Exposed"]
    Exposed -->|Quarantined|QuarantineProdromal["Quarantined<br>Prodromal"]

    Prodromal -->|Undetected|Rash
    Prodromal -->|Quarantined|Isolated
    Prodromal -->|Quarantined|QuarantineProdromal["Quarantined<br>Prodromal"]

    Rash -->|Detected|Isolated
    Rash -->|Detected|QuarantineRecovered["Quarantined<br>Recovered"]
    Rash -->|Detected or not|Hospitalized
    Rash -->|Undetected|Recovered

    Isolated -->|"End period (returns)"|Rash
    Isolated -->|End period|Recovered
    Isolated -->|Still isolated|QuarantineRecovered
    Isolated -->|End or not|Hospitalized

    QuarantineExposed -->|Ends period|Exposed
    QuarantineExposed -->|Ends period|Prodromal
    QuarantineExposed -->|Still in quarantine|QuarantineProdromal["Quarantined<br>Prodromal"]

    QuarantineSusceptible -->|Ends period|Susceptible

    QuarantineProdromal -->|Ends period|Prodromal
    QuarantineProdromal-->|Develops the disease|Isolated
    QuarantineProdromal-->|Ends period|Rash

    QuarantineRecovered -->|Ends period|Recovered

    Hospitalized -->Recovered
```

A simplified version aggregating quarantine states:

```mermaid
flowchart LR
    Susceptible -->|Starts quarantine| QuarantineS["Quarantine<br>Susceptible"]
    QuarantineS -->|Ends quarantine| Susceptible
    Susceptible --> Exposed

    Exposed     -->|Starts quarantine| Quarantine["Quarantine<br>or Isolated"]
    Quarantine  -->|Ends quarantine| Exposed
    Exposed     -->|Disease progression| Prodromal
    
    Prodromal   -->|Starts quarantine| Quarantine
    Quarantine  -->|Ends quarantine| Prodromal
    Prodromal   -->|Disease progression| Rash
    
    Rash        --> |Starts quarantine| Quarantine
    Quarantine  --> |Ends quarantine| Rash
    Rash        --> |Disease progression| Hospitalized
    Rash        --> |Disease progression| Recovered
    
    Quarantine  --> |Disease progression| Hospitalized
    Hospitalized--> |Disease progression| Recovered
    Hospitalized--> |Starts quarantine| Quarantine
    Quarantine  --> |Ends quarantine| Recovered
```

Individual transitions can be viewed in detail in [**03_individual_transitions.md**](./03_individual_transitions.md).


## Quarantine process

The quarantine is executed once throughout the simulation, and it is triggered as soon as an agent is detected in the rash period. Once that occurs, agents' states are updated according to the following process:

```mermaid
flowchart LR
    Start((Start)) --> infected{"Already<br>quarantined<br>or isolated?"}
    infected -->|Yes|End((End))
    infected -->|No|Rash{"Rash?"}
    Rash -->|Yes|Isolate((Isolate))
    Rash -->|No|vax
    vax{"Vaccinated?"}
    vax -->|Yes|End
    vax -->|No|WillQuarantine
    WillQuarantine{"Willing to<br>Quarantine?"} -->|No|End
    WillQuarantine -->|Yes|Quarantine((Quarantine))
```

## Transition probabilities

In the case of individuals with rash (Rash and Isolated states), they may become hospitalized or recover, each with "Hospitalization rate" and "1/Rash period" respectively.

In the case of the transmission process, the model computes the conditional probability of becoming infected from at most one other agent. For instance, an agent who contacts two other agents can either stay susceptible or become infected from either or the two, but it cannot become infected simulateneously from two different agents. Formally, the probability that agent $j$ transmits the disease to agent $i$ is given by:

```math
P(i\to j | \text{at most one event}) = \frac{
    p_i \times \prod_{k \ne i} (1 - p_k)}{
    \prod_k (1-p_k) + \sum_{k} p_k \times \prod_{l \ne k} (1 - p_l)
    }
```

Where $p_i$ is the unadjusted probability of becoming infected.

## Update susceptibles

Throughout the simulation, the contact rate is held constant, even when agents are isolated. The update schedule of susceptible agents is a function of the "available agents" of the model (excludes isolated, quarantined, or recovered). Thus, the number of contacts susceptible agents make is binomially distributed as follows:

```math
n \sim\text{Binomial}\left(N_{\text{available}}, \frac{C_{rate}}{N_{\text{available}}}\right)
```

<!-- # Notes from walkthrough

- Think about matrix combining states (ij) to simplify model.
~~- Rename vax_reduction_suscept to vax_efficacy.~~
~~- Replace day_detected -> day_flagged.~~
- Assuming that rash is forcefully isolated.
- Prodromal vax may skip quarantine.
~~- Change `quarantine_status` to `system_quarantine_status`.~~
- Change willigness to be fixed at the beginning.
~~- Double check the update_model (what's the order of the events?)?~~
- Add details about sampling from multiple viruses for update susceptible.
- Think about perfect detection, right now, if "Days undetected" == 0, then it's doing nothing.
- ~~Checkout update of rash who recovers but moves to quarantine.~~
~~- Double check the prodromal period move in update_quarantined_prodromal.~~
- Update isolated should also look at the quarantine days. -->