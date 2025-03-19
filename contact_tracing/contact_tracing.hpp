#define epiworld_double double

#include <iostream>
#include "../epiworld.hpp"

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

class ModelContactTracing: public Model<> {

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
        QUARANTINED_PRODROMAL,  //< In case of potential exposure
        RECOVERED                //< Recovered with permanent immunity
    };

    // Default constructor
    ModelContactTracing() {};

    ModelContactTracing(
        ModelContactTracing & model,
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
        epiworld_double days_undetected,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days,
        epiworld_fast_uint contact_tracing_days_back,
        epiworld_double contact_tracing_success_rate
    );

    ModelContactTracing(
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
        epiworld_double days_undetected,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days,
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
    int & contact_matrix_ij(int i, int j);

    void print_contact_tracing(int n_entries = -1);

    void reset();
    void update_available();

    Model<> * clone_ptr();

};

inline int & ModelContactTracing::contact_matrix_ij(int i, int j) {
    if (i > j)
        std::swap(i, j);
    return contact_matrix[j + i * this->size()];
}

inline void ModelContactTracing::print_contact_tracing(int n_entries) {
    if (n_entries < 0)
        n_entries = this->size();
    
    printf_epiworld("Contact matrix (first %i entires) (adjacency matrix)\n", n_entries);
    printf_epiworld("Each entry is the day of the contact between i and j, \n");
    printf_epiworld("with negative values indicating no contact \n");
    printf_epiworld("between the individuals.\n");
    for (int i = 0; i < n_entries; ++i)
    {
        for (int j = 0; j < n_entries; ++j)
        {
            printf_epiworld("% 4i ", contact_matrix_ij(i, j));
        }
        std::cout << std::endl;
    }
}

inline void ModelContactTracing::contact_tracing() {

    // Capturing the days that matter and the probability of success
    int days_back = this->par("How many days back to trace");
    epiworld_double success_rate = this->par("Contact tracing success rate");

    // Iterating through the new cases
    for (const auto & id: ids_triggered_contact_tracing)
    {
        // Iterating through the
        for (size_t i = 0u; i < size(); ++i) {

            // No contact
            if (contact_matrix_ij(id, i) == -1)
                continue;

            int days_since = today() - contact_matrix_ij(id, i);

            // Does it fit within the contact tracing window?
            if (days_since <= days_back) {

                // Do we succeed in the contact tracing?
                if (runif() < success_rate)
                {
                    // If the agent has a vaccine, then no need for quarantine
                    if (get_agent(i).get_n_tools() != 0u)
                        continue;

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
                            states::QUARANTINED_PRODROMAL
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
    auto * model = dynamic_cast<ModelContactTracing *>(m);
    model->update_available();
    model->contact_tracing();
    return;
}

inline void ModelContactTracing::reset() {
    
    // Initializing the population
    Model<>::agents_empty_graph(
        static_cast<size_t>(Model<>::operator()("Population size"))
    );
    
    Model<>::reset();

    // Cleaning contact matrix and list of who is quanrantined or isolated
    contact_matrix.resize(size() * size(), -1);
    std::fill(
        contact_matrix.begin(),
        contact_matrix.end(),
        -1
    );

    day_quarantined_or_isolated.resize(size(), 0);
    std::fill(
        day_quarantined_or_isolated.begin(),
        day_quarantined_or_isolated.end(),
        0);

    update_model(dynamic_cast<Model<>*>(this));
    return;
}

inline void ModelContactTracing::update_available() {

    this->available.clear();
    for (auto & agent: this->get_agents())
    {
        const auto & s = agent.get_state();
        if (s <= ModelContactTracing::states::RASH)
            this->available.push_back(&agent);
    }

    // Assumes fixed contact rate throughout the simulation
    this->set_rand_binom(
        static_cast<int>(this->available.size()),
        this->par("Contact rate")/static_cast<double>(this->size())
    );

}

inline Model<> * ModelContactTracing::clone_ptr()
{
        
    ModelContactTracing * ptr = new ModelContactTracing(
        *dynamic_cast<const ModelContactTracing*>(this)
        );

    return dynamic_cast< Model<> *>(ptr);

}

EPI_NEW_UPDATEFUN(update_susceptible, int) {

    int ndraw = m->rbinom();

    if (ndraw == 0)
        return;

    auto * model = dynamic_cast<ModelContactTracing *>(m);
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

        // The neighbor is infected because it is on the list!
        if (neighbor.get_virus() == nullptr)
            continue;

        // Only infectious individuals can transmit
        if (
            neighbor.get_state() != ModelContactTracing::states::PRODROMAL &&
            neighbor.get_state() != ModelContactTracing::states::RASH
        )
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
        p->change_state(m, ModelContactTracing::states::PRODROMAL);

    return;

};

EPI_NEW_UPDATEFUN(update_prodromal, int) {
    
    auto * model = dynamic_cast<ModelContactTracing *>(m);
    if (m->runif() < (1.0/m->par("Prodromal period")))
    {
        model->day_quarantined_or_isolated[p->get_id()] = m->today();
        p->change_state(m, ModelContactTracing::states::RASH);
    }

    return;

};

EPI_NEW_UPDATEFUN(update_rash, int) {

    auto * model = dynamic_cast<ModelContactTracing *>(m);
    int days_since_rash = m->today() - model->day_quarantined_or_isolated[p->get_id()];
    if (days_since_rash >= m->par("Days undetected"))
    {
        model->ids_triggered_contact_tracing.push_back(p->get_id());
        p->change_state(m, ModelContactTracing::states::ISOLATED);
    }
    
};

EPI_NEW_UPDATEFUN(update_quarantined_exposed, int) {

    // Extract the rate
    if (m->runif() < (1.0/p->get_virus()->get_incubation(m)))
        p->change_state(
            m,
            ModelContactTracing::states::QUARANTINED_PRODROMAL
        );

    return;

}

EPI_NEW_UPDATEFUN(update_quarantined_susceptible, int) {

    auto * model = dynamic_cast<ModelContactTracing *>(m);
    int days_since =
        m->today() - model->day_quarantined_or_isolated[p->get_id()];
    
    if (days_since >= m->par("Quarantine days"))
        p->change_state(m, ModelContactTracing::states::SUSECPTIBLE);

}

EPI_NEW_UPDATEFUN(update_quanrantined_prodromal, int) {

    auto * model = dynamic_cast<ModelContactTracing *>(m);
    
    // If they develop rash, then they are isolated and contact
    // tracing is triggered.
    if (m->runif() < (1.0/m->par("Prodromal period")))
    {
        model->ids_triggered_contact_tracing.push_back(p->get_id());
        p->change_state(m, ModelContactTracing::states::ISOLATED);
        return;
    }

    // Otherwise, these are moved to the prodromal period, if
    // the quanrantine period is over.
    int days_since =
        m->today() - model->day_quarantined_or_isolated[p->get_id()];
    
    if (days_since >= m->par("Quarantine days"))
        p->change_state(m, ModelContactTracing::states::PRODROMAL);

    return;

}

inline ModelContactTracing::ModelContactTracing(
    ModelContactTracing & model,
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
    epiworld_double days_undetected,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days,
    epiworld_fast_uint contact_tracing_days_back,
    epiworld_double contact_tracing_success_rate
) {

    model.add_state("Susceptible", update_susceptible);
    model.add_state("Exposed", update_exposed);
    model.add_state("Prodromal", update_prodromal);
    model.add_state("Rash", update_rash);
    model.add_state("Isolated", default_update_exposed<>);
    model.add_state("Quarantined Exposed", update_quarantined_exposed);
    model.add_state("Quarantined Susceptible", update_quarantined_susceptible);
    model.add_state("Quarantined Infectious", update_quanrantined_prodromal);
    model.add_state("Recovered");

    // Adding the model parameters
    model.add_param(contact_rate, "Contact rate");
    model.add_param(transmission_rate, "Transmission rate");
    model.add_param(incubation_period, "Incubation period");
    model.add_param(prodromal_period, "Prodromal period");
    model.add_param(1.0 / rash_period, "1/Rash period");
    model.add_param(days_undetected, "Days undetected");
    model.add_param(quarantine_days, "Quarantine days");
    model.add_param(contact_tracing_days_back, "How many days back to trace");
    model.add_param(
        contact_tracing_success_rate, "Contact tracing success rate"
    );
    model.add_param(prop_vaccinated, "Vaccination rate");
    model.add_param(vax_reduction_suscept, "Vax efficacy");
    model.add_param(vax_reduction_recovery_rate, "Vax improved recovery");

    // Designing the disease
    Virus<> measles("Measles");
    measles.set_state(states::EXPOSED, states::RECOVERED);
    measles.set_prob_infecting(&model("Transmission rate"));
    measles.set_prob_recovery(&model("1/Rash period"));
    measles.set_incubation(&model("Incubation period"));
    measles.set_distribution(
        distribute_virus_randomly(n_exposed, false)
    );

    model.add_virus(measles);

    // Designing the vaccine
    Tool<> vaccine("Vaccine");
    vaccine.set_susceptibility_reduction(&model("Vax efficacy"));
    vaccine.set_recovery_enhancer(&model("Vax improved recovery"));
    vaccine.set_distribution(
        distribute_tool_randomly(prop_vaccinated, true)
    );

    model.add_tool(vaccine);

    // Global actions
    model.add_globalevent(update_model, "Update model");
    model.queuing_off();

    // Setting the population
    model.agents_empty_graph(n);

    return;

}

inline ModelContactTracing::ModelContactTracing(
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
    epiworld_double days_undetected,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days,
    epiworld_fast_uint contact_tracing_days_back,
    epiworld_double contact_tracing_success_rate
) {

    ModelContactTracing(
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
        days_undetected,
        prop_vaccinated,
        quarantine_days,
        contact_tracing_days_back,
        contact_tracing_success_rate
    );

    return;

}


template<typename TSeq = EPI_DEFAULT_TSEQ>
void draw_mermaid_diagram(
    const epiworld::DataBase<TSeq> & db,
    const epiworld::Model<> & model,
    const std::string & filename = "",
    bool self = false
) {

    // Extracting the transition matrix
    auto tprob = db.transition_probability(false);
    auto states = model.get_states();

    std::vector< std::string > states_ids;
    for (size_t i = 0u; i < states.size(); ++i)
        states_ids.push_back("s" + std::to_string(i));

    std::string graph = "flowchart LR\n";

    // Declaring the states
    for (size_t i = 0u; i < states.size(); ++i)
    {
        graph += "\t" + states_ids[i] + "[" + states[i] + "]\n";
    }

    // Adding the transitions
    size_t n_states = states.size();
    for (size_t i = 0u; i < states.size(); ++i)
    {
        for (size_t j = 0u; j < states.size(); ++j)
        {
            if (!self && i == j)
                continue;

            if (tprob[i + j * n_states] > 0.0)
            {
                graph += "\t" + states_ids[i] + " -->|" + 
                    std::to_string(tprob[i + j * n_states]) + "| " + states_ids[j] + "\n";
            }
        }
    }

    if (filename != "")
    {
        std::ofstream file(filename);

        if (!file.is_open())
            throw std::runtime_error("Could not open the file for writing.");

        file << graph;
        file.close();
        
    } else {
        std::cout << graph << std::endl;
    }

    return;

}