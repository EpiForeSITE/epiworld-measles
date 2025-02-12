#include "epiworld.hpp"

using namespace epiworld;

class ModelMeaslesHighSchool: public Model<> {

private:

protected:
    enum states {
        SUSECPTIBLE,            //< Initial state
        EXPOSED,                //< Exposed (latent) to the disease
        INFECTIOUS,             //< Infectious (prodromal) state
        SYMPTOMATIC,            //< Infectious with rash
        QUARANTINED_EXPOSED,    //< In case of potential exposure
        QUARANTINED_UNEXPOSED,  //< In case of potential exposure
        QUARANTINED_INFECTIOUS, //< In case of potential exposure
        RECOVERED               //< Recovered with permanent immunity
    };

public:

    // Default constructor
    ModelMeaslesHighSchool() {};

    ModelMeaslesHighSchool(
        ModelMeaslesHighSchool & model,
        epiworld_fast_uint n,
        epiworld_fast_uint n_exposed,
        // Disease parameters
        epiworld_double transmission_rate,
        epiworld_double vax_reduction_suscept,
        epiworld_double vax_reduction_recovery_rate,
        epiworld_double incubation_period,
        epiworld_double prodromal_period,
        epiworld_double rash_period,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days_vax,
        epiworld_fast_uint quarantine_days_unvax,
        epiworld_fast_uint contact_tracing_depth,
        epiworld_double contact_tracing_success_rate
    );

    ModelMeaslesHighSchool(
        epiworld_fast_uint n,
        epiworld_fast_uint n_exposed,
        // Disease parameters
        epiworld_double transmission_rate,
        epiworld_double vax_reduction_suscept,
        epiworld_double vax_reduction_recovery_rate,
        epiworld_double incubation_period,
        epiworld_double prodromal_period,
        epiworld_double rash_period,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days_vax,
        epiworld_fast_uint quarantine_days_unvax,
        epiworld_fast_uint contact_tracing_depth,
        epiworld_double contact_tracing_success_rate
    );

    // ModelMeaslesHighSchool & run(
    //     epiworld_fast_uint ndays,
    //     int seed = -1
    // );


};

inline ModelMeaslesHighSchool::ModelMeaslesHighSchool(
    ModelMeaslesHighSchool & model,
    epiworld_fast_uint n,
    epiworld_fast_uint n_exposed,
    // Disease parameters
    epiworld_double transmission_rate,
    epiworld_double vax_reduction_suscept,
    epiworld_double vax_reduction_recovery_rate,
    epiworld_double incubation_period,
    epiworld_double prodromal_period,
    epiworld_double rash_period,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days_vax,
    epiworld_fast_uint quarantine_days_unvax,
    epiworld_fast_uint contact_tracing_depth,
    epiworld_double contact_tracing_success_rate
) {

    model.add_state("Susceptible");
    model.add_state("Exposed");
    model.add_state("Infectious");
    model.add_state("Symptomatic");
    model.add_state("Quarantined Exposed");
    model.add_state("Quarantined Unexposed");
    model.add_state("Quarantined Infectious");
    model.add_state("Recovered");

    // Adding the model parameters
    model.add_param(transmission_rate, "Transmission rate");
    model.add_param(
        vax_reduction_suscept, "Vaccine reduction in susceptibility"
    );
    model.add_param(
        vax_reduction_recovery_rate, "Vaccine reduction in recovery rate"
    );
    model.add_param(incubation_period, "Incubation period");
    model.add_param(prodromal_period, "Prodromal period");
    model.add_param(rash_period, "Rash period");
    model.add_param(quarantine_days_vax, "Quarantine days vax");
    model.add_param(quarantine_days_unvax, "Quarantine days unvax");
    model.add_param(contact_tracing_depth, "Contact tracing depth");
    model.add_param(
        contact_tracing_success_rate, "Contact tracing success rate"
    );

    // Designing the disease
    Virus<> measles("Measles");
    measles.set_state(states::EXPOSED, states::RECOVERED);
    measles.set_prob_infecting(transmission_rate);
    measles.set_prob_recovery(1.0 / rash_period);
    measles.set_incubation(incubation_period);
    measles.set_distribution(
        distribute_virus_randomly(n_exposed, false)
    );

    model.add_virus(measles);

    // Designing the vaccine
    Tool<> vaccine("Vaccine");
    vaccine.set_susceptibility_reduction(vax_reduction_suscept);
    vaccine.set_recovery_enhancer(vax_reduction_recovery_rate);
    vaccine.set_distribution(
        distribute_tool_randomly(prop_vaccinated, true)
    );

    model.add_tool(vaccine);

    // Initializing the population
    model.agents_empty_graph(n);

    return;

}

inline ModelMeaslesHighSchool::ModelMeaslesHighSchool(
    epiworld_fast_uint n,
    epiworld_fast_uint n_exposed,
    // Disease parameters
    epiworld_double transmission_rate,
    epiworld_double vax_reduction_suscept,
    epiworld_double vax_reduction_recovery_rate,
    epiworld_double incubation_period,
    epiworld_double prodromal_period,
    epiworld_double rash_period,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days_vax,
    epiworld_fast_uint quarantine_days_unvax,
    epiworld_fast_uint contact_tracing_depth,
    epiworld_double contact_tracing_success_rate
) {

    ModelMeaslesHighSchool(
        *this,
        n,
        n_exposed,
        transmission_rate,
        vax_reduction_suscept,
        vax_reduction_recovery_rate,
        incubation_period,
        prodromal_period,
        rash_period,
        prop_vaccinated,
        quarantine_days_vax,
        quarantine_days_unvax,
        contact_tracing_depth,
        contact_tracing_success_rate
    );

    return;

}

// Defining the 

int main() {
    ModelMeaslesHighSchool model(
        1000, // Population size
        10,   // initial number of exposed
        0.95, // Transmission rate
        0.9,  // Vaccine reduction in susceptibility
        0.5,  // Vaccine reduction in recovery rate
        5,    // Incubation period
        5,    // Prodromal period
        5,    // Rash period
        .8,   // Vaccination rate
        14,   // Quarantine days for vaccinated
        14,   // Quarantine days for unvaccinated
        5,    // Contact tracing depth
        0.9   // Contact tracing success rate
    );
    model.print();
    return 0;
}

