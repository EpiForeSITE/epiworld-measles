#include "highschool.hpp"

// Defining the 

int main(int argc, char *argv[]) {

    // Passing the single argument to this function
    std::string fn = "highschool-params.yaml";
    if (argc > 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    } else if (argc == 2) {
        fn = std::string(argv[1]);
    }
    std::cout << "Using file: " << fn << std::endl;

    ModelMeaslesHighSchool model(
        1000, // Population size
        1,    // initial number of exposed
        10,   // Contact rate  
        0.95, // Transmission rate
        0.98, // Vaccine reduction in susceptibility
        0.5,  // Vaccine reduction in recovery rate
        5,    // Incubation period
        4,    // Prodromal period
        4,    // Rash period
        2,    // Max days in rash
        .9,   // Vaccination rate
        14,   // Quarantine days for vaccinated
        14,    // Days back to trace
        0.9   // Contact tracing success rate
    );

    model.read_params(fn);

    // model.run(60, 331);

    auto saver = make_save_run<int>(
        "highschool-out/%03lu",
        true, false, false, false, false, true, true, false, false
    );

    model.run_multiple(
        static_cast<int>(model("N days")), // Number of days
        static_cast<int>(model("Replicates")), // Number of sims
        static_cast<int>(model("Seed")), // Seed
        saver,
        true,
        true,
        static_cast<int>(model("Threads"))
    );

    model.print();
    model.print_contact_tracing(10);
    return 0;

}
