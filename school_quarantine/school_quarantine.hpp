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

class ModelSchoolQuarantine: public Model<> {

private:

public:

    static const epiworld_fast_uint SUSECPTIBLE             = 0u;
    static const epiworld_fast_uint EXPOSED                 = 1u;
    static const epiworld_fast_uint PRODROMAL               = 2u;
    static const epiworld_fast_uint RASH                    = 3u;
    static const epiworld_fast_uint ISOLATED                = 4u;
    static const epiworld_fast_uint QUARANTINED_EXPOSED     = 5u;
    static const epiworld_fast_uint QUARANTINED_SUSCEPTIBLE = 6u;
    static const epiworld_fast_uint QUARANTINED_PRODROMAL   = 7u;
    static const epiworld_fast_uint QUARANTINED_RECOVERED   = 8u;
    static const epiworld_fast_uint HOSPITALIZED            = 9u;
    static const epiworld_fast_uint RECOVERED               = 10u;
    
    // Default constructor
    ModelSchoolQuarantine() {};

    ModelSchoolQuarantine(
        ModelSchoolQuarantine & model,
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
        epiworld_double hospitalization_rate,
        epiworld_double hospitalization_duration,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days,
        epiworld_double contact_tracing_success_rate
    );

    ModelSchoolQuarantine(
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
        epiworld_double hospitalization_rate,
        epiworld_double hospitalization_duration,
        // Policy parameters
        epiworld_double prop_vaccinated,
        epiworld_fast_uint quarantine_days,
        epiworld_double contact_tracing_success_rate
    );

    
    std::vector<epiworld::Agent<> *> available;   

    enum class QuarantineStatus {
        INACTIVE,
        TRIGGERED,
        ACTIVE
    };

    QuarantineStatus quarantine_status;
    std::vector< int > day_quarantined_or_isolated;
    
    void quarantine_agents();

    void reset();
    void update_available();

    Model<> * clone_ptr();

};

inline void ModelSchoolQuarantine::quarantine_agents() {

    // Iterating through the new cases
    if (quarantine_status != QuarantineStatus::TRIGGERED)
        return;

    // Capturing the days that matter and the probability of success
    epiworld_double willingness = this->par("Quarantine willingness");

    // Iterating through the
    for (size_t i = 0u; i < size(); ++i) {

        auto agent_state = get_agent(i).get_state();

        // Already quarantined or isolated
        if (agent_state > RASH)
            continue;

        // Rash is evident, so they go straight to isolation, in all
        // cases.
        if (agent_state == RASH)
        {
            get_agent(i).change_state(this, ISOLATED);
            day_quarantined_or_isolated[i] = today();
            continue;
        }

        // If the agent has a vaccine, then no need for quarantine
        if (get_agent(i).get_n_tools() != 0u)
            continue;


        // Quarantine will depend on the willingness of the agent
        // to be quarantined. If negative, then quarantine never happens.
        if (runif() < willingness)
        {

            if (agent_state == SUSECPTIBLE)
                get_agent(i).change_state(this, QUARANTINED_SUSCEPTIBLE);
            else if (agent_state == EXPOSED)
                get_agent(i).change_state(this, QUARANTINED_EXPOSED);
            else if (agent_state == PRODROMAL)
                get_agent(i).change_state(this, QUARANTINED_PRODROMAL);

            // And we add the day of quarantine
            day_quarantined_or_isolated[i] = today();

        }

    }

    // Clearing the list of ids
    quarantine_status = QuarantineStatus::ACTIVE;

    return;

}

EPI_NEW_GLOBALFUN(update_model, int) {
    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    model->update_available();
    model->quarantine_agents();
    return;
}

inline void ModelSchoolQuarantine::reset() {
    
    Model<>::reset();

    quarantine_status = QuarantineStatus::INACTIVE;
        
    day_quarantined_or_isolated.resize(size(), 0);
    std::fill(
        day_quarantined_or_isolated.begin(),
        day_quarantined_or_isolated.end(),
        0);

    update_model(dynamic_cast<Model<>*>(this));
    return;
    
}

inline void ModelSchoolQuarantine::update_available() {

    this->available.clear();
    for (auto & agent: this->get_agents())
    {
        const auto & s = agent.get_state();
        if (s <= RASH)
            this->available.push_back(&agent);
    }

    // Assumes fixed contact rate throughout the simulation
    this->set_rand_binom(
        static_cast<int>(this->available.size()),
        this->par("Contact rate")/ static_cast<double>(available.size())// static_cast<double>(this->size())
    );

}

inline Model<> * ModelSchoolQuarantine::clone_ptr()
{
        
    ModelSchoolQuarantine * ptr = new ModelSchoolQuarantine(
        *dynamic_cast<const ModelSchoolQuarantine*>(this)
        );

    return dynamic_cast< Model<> *>(ptr);

}

EPI_NEW_UPDATEFUN(update_susceptible, int) {

    // How many contacts to draw
    int ndraw = m->rbinom();

    if (ndraw == 0)
        return;

    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    size_t n_available = model->available.size();

    if (n_available == 0)
        return;

    // Drawing from the set
    int nviruses_tmp = 0;
    int i = 0;
    while (i < ndraw)
    {
        // Picking the actual contacts
        int which = static_cast<int>(std::floor(n_available * m->runif()));

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

        // We successfully drew a contact, so we increment the counter
        i++;

        // No virus, no way to transmit
        if (neighbor.get_virus() == nullptr)
            continue;

        // Only infectious individuals can transmit
        if (
            (neighbor.get_state() != model->PRODROMAL) &&
            (neighbor.get_state() != model->RASH)
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
        p->change_state(m, ModelSchoolQuarantine::PRODROMAL);

    return;

};

EPI_NEW_UPDATEFUN(update_prodromal, int) {
    
    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    if (m->runif() < (1.0/m->par("Prodromal period")))
    {
        p->change_state(m, ModelSchoolQuarantine::RASH);
    }

    return;

};

EPI_NEW_UPDATEFUN(update_rash, int) {

    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    
    #ifdef EPI_DEBUG
    if (model->day_quarantined_or_isolated.size() <= p->get_id())
        throw std::logic_error(
            "The agent is not in the list of quarantined or isolated agents: " +
            std::to_string(p->get_id()) +
            " vs " +
            std::to_string(model->day_quarantined_or_isolated.size()) +
            ". The model has " + std::to_string(model->size()) + " agents."
        );
    #endif

    // Checking if the agent will be detected or not
    // How many days since detected  
    bool isolate = false;
    if (
        (m->par("Days undetected") > 0) &&
        (m->runif() > 1.0/m->par("Days undetected"))
    )
    {
        model->quarantine_status =  
            ModelSchoolQuarantine::QuarantineStatus::TRIGGERED;

        isolate = true;
    }

    // Probability of Staying in the rash period vs becoming
    // hospitalized
    m->array_double_tmp[0] = 1.0/m->par("Rash days");
    m->array_double_tmp[1] = m->par("Hospitalization rate");

    #ifdef EPI_DEBUG
    EPI_DEBUG_PRINTF(
        "Rash days: %f, Hospitalization rate: %f\n",
        m->array_double_tmp[0],
        m->array_double_tmp[1]
    );
    #endif

    int which = epiworld::roulette(2, m);

    // Recovers
    if (which == -1)
    {
        if (isolate)
        {
            model->day_quarantined_or_isolated[p->get_id()] = m->today();
            p->rm_agent_by_virus(
                m,
                ModelSchoolQuarantine::QUARANTINED_RECOVERED
            );
        }
        else
            p->rm_agent_by_virus(m, ModelSchoolQuarantine::RECOVERED);
    }
    else if (which == 1)
    {
        // If hospitalized, then the agent is removed from the system
        // effectively
        p->change_state(m, ModelSchoolQuarantine::HOSPITALIZED);
    } else
    {
        throw std::logic_error("The roulette returned an unexpected value.");
    }

    
};

EPI_NEW_UPDATEFUN(update_quarantined_exposed, int) {

    // How many days since quarantine started
    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    int days_since =
        m->today() - model->day_quarantined_or_isolated[p->get_id()];

    bool unquarantine =
        (m->par("Quarantine days") <= days_since) ?
        true: false;

    // Will develop prodromal symptoms?
    if (m->runif() < (1.0/p->get_virus()->get_incubation(m)))
    {
        // If the quarantine period is over, then they are moved to
        // the prodromal period. Otherwise, they are moved to the
        // quarantined prodromal period.
        if (unquarantine)
            p-> change_state(
                m,
                ModelSchoolQuarantine::PRODROMAL
            );
        else
            p->change_state(
                m,
                ModelSchoolQuarantine::QUARANTINED_PRODROMAL
            );

    }

}

EPI_NEW_UPDATEFUN(update_quarantined_susceptible, int) {

    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    int days_since =
        m->today() - model->day_quarantined_or_isolated[p->get_id()];
    
    if (days_since >= m->par("Quarantine days"))
        p->change_state(m, ModelSchoolQuarantine::SUSECPTIBLE);

}

EPI_NEW_UPDATEFUN(update_quanrantined_prodromal, int) {

    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    
    // If they develop rash, then they are isolated and contact
    // tracing is triggered.
    if (m->runif() < (1.0/m->par("Prodromal period")))
    {

        p->change_state(m, ModelSchoolQuarantine::ISOLATED);
        return;

    }

    // Otherwise, these are moved to the prodromal period, if
    // the quanrantine period is over.
    int days_since =
        m->today() - model->day_quarantined_or_isolated[p->get_id()];
    
    if (days_since >= m->par("Quarantine days"))
        p->change_state(m, ModelSchoolQuarantine::PRODROMAL);


}

EPI_NEW_UPDATEFUN(update_quarantined_recovered, int) {

    auto * model = dynamic_cast<ModelSchoolQuarantine *>(m);
    int days_since =
        m->today() - model->day_quarantined_or_isolated[p->get_id()];
    
    if (days_since >= m->par("Quarantine days"))
        p->change_state(m, ModelSchoolQuarantine::RECOVERED);

}

EPI_NEW_UPDATEFUN(update_hospitalized, int) {

    // The agent is removed from the system
    if (m->runif() < 1.0/m->par("Hospitalization days"))
        p->rm_agent_by_virus(m, ModelSchoolQuarantine::RECOVERED);

    return;

}

EPI_NEW_UPDATEFUN(update_isolated, int) {

    // Probability of staying in the rash period vs becoming
    // hospitalized
    m->array_double_tmp[0] = 1.0/m->par("Rash days");
    m->array_double_tmp[1] = m->par("Hospitalization rate");

    int which = epiworld::roulette(2, m);

    // Recovers
    if (which == -1)
    {
        // Recovers. It doesn't affect if quarantine has been triggered,
        // since recovered agents effectively leave the system.
        p->rm_agent_by_virus(m, ModelSchoolQuarantine::RECOVERED);
    }
    else if (which == 1)
    {
        // If hospitalized, then the agent is removed from the system
        // effectively
        p->change_state(m, ModelSchoolQuarantine::HOSPITALIZED);
    } else
    {
        throw std::logic_error("The roulette returned an unexpected value.");
    }

}

inline ModelSchoolQuarantine::ModelSchoolQuarantine(
    ModelSchoolQuarantine & model,
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
    epiworld_double hospitalization_rate,
    epiworld_double hospitalization_duration,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days,
    epiworld_double contact_tracing_success_rate
) {

    model.add_state("Susceptible", update_susceptible);
    model.add_state("Exposed", update_exposed);
    model.add_state("Prodromal", update_prodromal);
    model.add_state("Rash", update_rash);
    model.add_state("Isolated", update_isolated);
    model.add_state("Quarantined Exposed", update_quarantined_exposed);
    model.add_state("Quarantined Susceptible", update_quarantined_susceptible);
    model.add_state("Quarantined Infectious", update_quanrantined_prodromal);
    model.add_state("Quarantined Recovered", update_quarantined_exposed);
    model.add_state("Hospitalized", update_hospitalized);
    model.add_state("Recovered");

    // Adding the model parameters
    model.add_param(contact_rate, "Contact rate");
    model.add_param(transmission_rate, "Transmission rate");
    model.add_param(incubation_period, "Incubation period");
    model.add_param(prodromal_period, "Prodromal period");
    model.add_param(rash_period, "Rash days");
    model.add_param(days_undetected, "Days undetected");
    model.add_param(quarantine_days, "Quarantine days");
    model.add_param(
        contact_tracing_success_rate, "Quarantine willingness"
    );
    model.add_param(hospitalization_rate, "Hospitalization rate");
    model.add_param(hospitalization_duration, "Hospitalization days");
    model.add_param(prop_vaccinated, "Vaccination rate");
    model.add_param(vax_reduction_suscept, "Vax efficacy");
    model.add_param(vax_reduction_recovery_rate, "Vax improved recovery");

    // Designing the disease
    Virus<> measles("Measles");
    measles.set_state(EXPOSED, RECOVERED);
    measles.set_prob_infecting(&model("Transmission rate"));
    measles.set_prob_recovery(&model("Rash days"));
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

inline ModelSchoolQuarantine::ModelSchoolQuarantine(
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
    epiworld_double hospitalization_rate,
    epiworld_double hospitalization_duration,
    // Policy parameters
    epiworld_double prop_vaccinated,
    epiworld_fast_uint quarantine_days,
    epiworld_double contact_tracing_success_rate
) {

    ModelSchoolQuarantine(
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
        hospitalization_rate,
        hospitalization_duration,
        prop_vaccinated,
        quarantine_days,
        contact_tracing_success_rate
    );

    return;

}
