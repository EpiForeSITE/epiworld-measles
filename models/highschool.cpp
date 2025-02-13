#define epiworld_double double

#include <iostream>
#include "epiworld.hpp"

/**
 * Notes:
 * - Recall they may have 1 or 2 dozes of the vaccine.
 * - Vaccinated individuals will not go to the quarantine state.
 * - Recovery rates may not make a difference as they are either isolated
 * or out.
 * - Rash may take a couple of days to detect.
 * - Data that the econ component should capture:
 *   - Number of detected cases.
 *   - Number of isolated and quarantined cases.
 *   - Hospitalized individuals will be a proportion of the cases.
 * - Depth does not make sense in this case. 
 */

using namespace epiworld;

class ModelMeaslesHighSchool: public Model<> {

private:

public:

    enum states {
        SUSECPTIBLE,             //< Initial state
        EXPOSED,                 //< Exposed (latent) to the disease
        PRODROMAL,               //< Infectious (prodromal) state
        RASH,                    //< Infectious with rash
        ISOLATED,                //< Isolated
        QUARANTINED_EXPOSED,     //< In case of potential exposure
        QUARANTINED_SUSCEPTIBLE, //< In case of potential exposure
        QUARANTINED_INFECTIOUS,  //< In case of potential exposure
        RECOVERED                //< Recovered with permanent immunity
    };

    // Default constructor
    ModelMeaslesHighSchool() {};

    ModelMeaslesHighSchool(
        ModelMeaslesHighSchool & model,
        epiworld_fast_uint n,
        epiworld_fast_uint n_exposed,
        // Disease parameters
        epiworld_double contact_rate,
        epiworld_double transmission_rate,
        epiworld_double vax_reduction_suscept,
        epiworld_double vax_reduction_recovery_rate,
        epiworld_double incubation_period,
        epiworld_double prodromal_period,
        epiworld_double rash_period,
        epiworld_double max_days_in_rash,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days_vax,
        epiworld_fast_uint quarantine_days_unvax,
        epiworld_fast_uint contact_tracing_days_back,
        epiworld_double contact_tracing_success_rate
    );

    ModelMeaslesHighSchool(
        epiworld_fast_uint n,
        epiworld_fast_uint n_exposed,
        // Disease parameters
        epiworld_double contact_rate,
        epiworld_double transmission_rate,
        epiworld_double vax_reduction_suscept,
        epiworld_double vax_reduction_recovery_rate,
        epiworld_double incubation_period,
        epiworld_double prodromal_period,
        epiworld_double rash_period,
        epiworld_double max_days_in_rash,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days_vax,
        epiworld_fast_uint quarantine_days_unvax,
        epiworld_fast_uint contact_tracing_days_back,
        epiworld_double contact_tracing_success_rate
    );

    
    std::vector<epiworld::Agent<> *> available;
    std::vector< int > ids_triggered_contact_tracing;
    std::vector< int > day_quarantined_or_isolated;
    
    void contact_tracing();
    
    /**
     * Each (i,j) entry represents is -1 if there's no contact
     * between the pair, otherwise it is the day of the contact.
     * Contact tracers will ask for contact in the last x-days.
     * 
     * The matrix is row-major order and symmetric.
     */
    std::vector< int > contact_matrix; //< n x n matrix
    int & contact_matrix_ij(int i, int j) {
        return contact_matrix[j + i * this->size()];
    }

    void print_contact_tracing(int n_entries = -1) {
        if (n_entries < 0)
            n_entries = this->size();
        for (int i = 0; i < n_entries; ++i)
        {
            for (int j = 0; j < n_entries; ++j)
            {
                std::cout << contact_matrix_ij(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    void reset();
    void update_available();

    // ModelMeaslesHighSchool & run(
    //     epiworld_fast_uint ndays,
    //     int seed = -1
    // );


};

inline void ModelMeaslesHighSchool::contact_tracing() {

    // Capturing the days that matter and the probability of success
    int days_back = this->par("How many days back to trace");
    epiworld_double success_rate = this->par("Contact tracing success rate");

    // Iterating through the new cases
    for (const auto & id: ids_triggered_contact_tracing)
    {
        // Iterating through the
        for (size_t i = 0u; i < size(); ++i) {
            int days_since = today() - contact_matrix_ij(id, i);

            // Does it fit within the contact tracing window?
            if (days_since <= days_back) {

                // Do we succeed in the contact tracing?
                if (runif() < success_rate)
                {
                    // Is it already in quarantine or isolated?
                    if (get_agent(i).get_state() >= states::RASH)
                        continue;

                    if (get_agent(i).get_state() == states::SUSECPTIBLE)
                        get_agent(i).change_state(
                            this, states::QUARANTINED_SUSCEPTIBLE
                        );
                    else if (get_agent(i).get_state() == states::EXPOSED)
                        get_agent(i).change_state(
                            this,
                            states::QUARANTINED_EXPOSED
                        );
                    else if (get_agent(i).get_state() == states::PRODROMAL)
                        get_agent(i).change_state(
                            this,
                            states::QUARANTINED_INFECTIOUS
                        );

                    // And we add the day of quarantine
                    day_quarantined_or_isolated[i] = today();
                }

            }

        }

    }

    // Clearing the list of ids
    ids_triggered_contact_tracing.clear();

    return;

}

EPI_NEW_GLOBALFUN(update_model, int) {
    auto * model = dynamic_cast<ModelMeaslesHighSchool *>(m);
    model->update_available();
    model->contact_tracing();
    return;
}

inline void ModelMeaslesHighSchool::reset() {
    Model<>::reset();
    update_model(dynamic_cast<Model<>*>(this));
    return;
}

inline void ModelMeaslesHighSchool::update_available() {

    this->available.clear();
    for (auto & agent: this->get_agents())
    {
        const auto & s = agent.get_state();
        if (s <= ModelMeaslesHighSchool::states::RASH)
            this->available.push_back(&agent);
    }

    this->set_rand_binom(
        static_cast<int>(this->available.size()),
        this->par("Contact rate")/static_cast<double>(this->size())
    );

}



EPI_NEW_GLOBALFUN(quarantine, int) {

    // Checking the rash cases

}

EPI_NEW_UPDATEFUN(update_susceptible, int) {

    int ndraw = m->rbinom();

    if (ndraw == 0)
        return;

    auto * model = dynamic_cast<ModelMeaslesHighSchool *>(m);
    size_t n_available = model->available.size();

    if (n_available == 0)
        return;

    // Drawing from the set
    int nviruses_tmp = 0;
    for (int i = 0; i < ndraw; ++i)
    {
        // Now selecting who is transmitting the disease
        int which = static_cast<int>(
            std::floor(n_available * m->runif())
        );

        /* There is a bug in which runif() returns 1.0. It is rare, but
            * we saw it here. See the Notes section in the C++ manual
            * https://en.cppreference.com/mwiki/index.php?title=cpp/numeric/random/uniform_real_distribution&oldid=133329
            * And the reported bug in GCC:
            * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=63176
            * 
            */
        if (which == static_cast<int>(n_available))
            --which;

        epiworld::Agent<> & neighbor = *model->available[which];

        // Can't sample itself
        if (neighbor.get_id() == p->get_id())
            continue;

        // Adding an entry for contact tracing
        model->contact_matrix_ij(p->get_id(), neighbor.get_id()) = m->today();
        model->contact_matrix_ij(neighbor.get_id(), p->get_id()) = m->today();

        // The neighbor is infected because it is on the list!
        if (neighbor.get_virus() == nullptr)
            continue;

        auto & v = neighbor.get_virus();

        #ifdef EPI_DEBUG
        if (nviruses_tmp >= static_cast<int>(m->array_virus_tmp.size()))
            throw std::logic_error("Trying to add an extra element to a temporal array outside of the range.");
        #endif
            
        /* And it is a function of susceptibility_reduction as well */ 
        m->array_double_tmp[nviruses_tmp] =
            (1.0 - p->get_susceptibility_reduction(v, m)) * 
            v->get_prob_infecting(m) * 
            (1.0 - neighbor.get_transmission_reduction(v, m)) 
            ; 
    
        m->array_virus_tmp[nviruses_tmp++] = &(*v);
            
    }

    // No virus to compute
    if (nviruses_tmp == 0u)
        return;

    // Running the roulette
    int which = roulette(nviruses_tmp, m);

    if (which < 0)
        return;

    p->set_virus(*m->array_virus_tmp[which], m);

    return; 

};

EPI_NEW_UPDATEFUN(update_exposed, int) {


    // Extract the rate
    if (m->runif() < (1.0/p->get_virus()->get_incubation(m)))
        p->change_state(m, ModelMeaslesHighSchool::states::PRODROMAL);

    return;

};

EPI_NEW_UPDATEFUN(update_prodromal, int) {
    
    auto * model = dynamic_cast<ModelMeaslesHighSchool *>(m);
    if (m->runif() < (1.0/m->par("Prodromal period")))
    {
        model->day_quarantined_or_isolated[p->get_id()] = m->today();
        p->change_state(m, ModelMeaslesHighSchool::states::RASH);
    }

    return;

};

EPI_NEW_UPDATEFUN(update_rash, int) {

    auto * model = dynamic_cast<ModelMeaslesHighSchool *>(m);
    int days_since_rash = m->today() - model->day_quarantined_or_isolated[p->get_id()];
    if (days_since_rash >= m->par("Max days in rash"))
    {
        model->ids_triggered_contact_tracing.push_back(p->get_id());
        p->change_state(m, ModelMeaslesHighSchool::states::ISOLATED);
    }
    
};


inline ModelMeaslesHighSchool::ModelMeaslesHighSchool(
    ModelMeaslesHighSchool & model,
    epiworld_fast_uint n,
    epiworld_fast_uint n_exposed,
    // Disease parameters
    epiworld_double contact_rate,
    epiworld_double transmission_rate,
    epiworld_double vax_reduction_suscept,
    epiworld_double vax_reduction_recovery_rate,
    epiworld_double incubation_period,
    epiworld_double prodromal_period,
    epiworld_double rash_period,
    epiworld_double max_days_in_rash,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days_vax,
    epiworld_fast_uint quarantine_days_unvax,
    epiworld_fast_uint contact_tracing_days_back,
    epiworld_double contact_tracing_success_rate
) {

    model.add_state("Susceptible", update_susceptible);
    model.add_state("Exposed", update_exposed);
    model.add_state("Prodromal", update_prodromal);
    model.add_state("Rash", update_rash);
    model.add_state("Isolated", default_update_exposed<>);
    model.add_state("Quarantined Exposed");
    model.add_state("Quarantined Unexposed");
    model.add_state("Quarantined Infectious");
    model.add_state("Recovered");

    // Adding the model parameters
    model.add_param(contact_rate, "Contact rate");
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
    model.add_param(max_days_in_rash, "Max days in rash");
    model.add_param(quarantine_days_vax, "Quarantine days vax");
    model.add_param(quarantine_days_unvax, "Quarantine days unvax");
    model.add_param(contact_tracing_days_back, "How many days back to trace");
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
    model.contact_matrix.resize(n * n, 0);
    model.day_quarantined_or_isolated.resize(n, 0);

    // Global actions
    model.add_globalevent(update_model, "Update model");
    model.queuing_off();


    return;

}

inline ModelMeaslesHighSchool::ModelMeaslesHighSchool(
    epiworld_fast_uint n,
    epiworld_fast_uint n_exposed,
    // Disease parameters
    epiworld_double contact_rate,
    epiworld_double transmission_rate,
    epiworld_double vax_reduction_suscept,
    epiworld_double vax_reduction_recovery_rate,
    epiworld_double incubation_period,
    epiworld_double prodromal_period,
    epiworld_double rash_period,
    epiworld_double max_days_in_rash,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days_vax,
    epiworld_fast_uint quarantine_days_unvax,
    epiworld_fast_uint contact_tracing_days_back,
    epiworld_double contact_tracing_success_rate
) {

    ModelMeaslesHighSchool(
        *this,
        n,
        n_exposed,
        contact_rate,
        transmission_rate,
        vax_reduction_suscept,
        vax_reduction_recovery_rate,
        incubation_period,
        prodromal_period,
        rash_period,
        max_days_in_rash,
        prop_vaccinated,
        quarantine_days_vax,
        quarantine_days_unvax,
        contact_tracing_days_back,
        contact_tracing_success_rate
    );

    return;

}

// Defining the 

int main() {
    ModelMeaslesHighSchool model(
        1000, // Population size
        1,    // initial number of exposed
        5,   // Contact rate  
        0.95, // Transmission rate
        0.9,  // Vaccine reduction in susceptibility
        0.5,  // Vaccine reduction in recovery rate
        5,    // Incubation period
        4,    // Prodromal period
        4,    // Rash period
        2,    // Max days in rash
        .9,   // Vaccination rate
        14,   // Quarantine days for vaccinated
        14,   // Quarantine days for unvaccinated
        5,    // Contact tracing depth
        0.9   // Contact tracing success rate
    );
    model.run(60, 331);
    model.print();
    model.print_contact_tracing(10);
    return 0;
}

