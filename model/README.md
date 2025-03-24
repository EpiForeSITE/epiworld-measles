# Measles model

The model implemented in this repository is a time-discrete Agent-Based Model [ABM]. The model assumes perfect mixing (*i.e.*, no fixed contact network but a system where all agents can interact with each other). The implementation is done using the [epiworld C++ library](https://github.com/UofUEpiBio/epiworld), which we also maintain. Here is a description of the files included in this folder:

- [`school_quarantine.hpp`](./school_quarantine.hpp): Is a C++ header file that contains the model implementation using epiworld. The model is implemented as the class `ModelSchoolQuarantine`.

- [`school_quarantine.cpp`](./school_quarantine.cpp): Is the main program that runs the model. This depends on the implementation in the header file, and takes two arguments: (a) A filepath to the model parameters in `yaml` and (b) a filepath to the output directory. The program runs multiple replicates of the model and saves them in the output directory.

- [**01_r0_of_measles.md**](./01_r0_of_measles.md): A description of the R0 of measles and an example showing the different outbreak sizes as a function of various vaccination rates.

- [**02_model_details.md**](./02_model_details.md): A detailed description of the model, including the model's assumptions, implementation using epiworld, and detailed description of the model's states and transitions.

- [**03_individual_transitions.md**](./03_individual_transitions.md): A detailed description of the individual transitions in the model.