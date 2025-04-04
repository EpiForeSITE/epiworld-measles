[![ForeSITE Group](https://raw.githubusercontent.com/EpiForeSITE/software/e82ed88f75e0fe5c0a1a3b38c2b94509f122019c/docs/assets/foresite-software-badge.svg)](https://github.com/EpiForeSITE) [![Run all scenarios](https://github.com/EpiForeSITE/epiworld-measles/actions/workflows/run_all.yaml/badge.svg)](https://github.com/EpiForeSITE/epiworld-measles/actions/workflows/run_all.yaml)

# Scenario modeling of measles using agent-based modeling

> [!CAUTION]
> This project is a work in progress. Use it at your own risk. **This model simulates a single school, so community transmission is not included**

> [!CAUTION]
> The code and model are still under development. We would love to hear your feedback and suggestions. Please open an issue in the [GitHub repository](https://github.com/EpiForeSITE/epiworld-measles) if you have any questions or suggestions.

This project aims to generate an ABM using [epiworld](https://github.com/UofUEpiBio/epiworld) to do scenario modeling of measles. The contents of this project follow:

- **Model details**: A detailed description of the model, including the model's assumptions, implementation using epiworld, and detailed description can be found in the [**model**](./model/README.md) folder.

- **Scenarios**: A description of the different scenarios and their results can be found in the [**scenarios**](./scenarios/README.md) folder.

Both folders include a `Makefile` that provides a handful of targets users can call. Within the command line, users can type `make` inside each folder and get a description of the available targets.


## Overview of the model

This agent-based model [ABM] has the following states:

``` mermaid
flowchart LR
  Susceptible --> Exposed
  Exposed --> Prodromal
  Prodromal --> Rash
  Rash --> Hospitalized
  Rash --> Recovered
  Hospitalized --> Recovered
```

The model includes a quarantine process that is triggered when an agent develops a rash. The quarantine process includes the following steps:

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

More details can be found in the [**model details**](./model/README.md) folder.


## Development

The development of the project has been carried out using a [Development Container](https://containers.dev). All the configuration is available under the [`.devcontainer` folder](./.devcontainer). The image is described in the file [`.devcontainer/ContainerFile`](./.devcontainer/ContainerFile). The image is available on GitHub under `ghcr.io/epiforesite/epiworld-measles:latest`.




