#include "school_quarantine.hpp"

// Defining the 

int main(int argc, char *argv[]) {

    // Passing the single argument to this function
    std::string fn      = "new_bridge_88_params.yaml";
    size_t n            = 1000;
    std::string out_dir = "new_bridge_88";
    if (argc != 4 && argc != 1) {
        std::cerr << "Usage: " << argv[0] << " <filename> <outdir> <size>" << std::endl;
        return 1;
    } else if (argc == 4) {
        fn      = std::string(argv[1]);
        out_dir = std::string(argv[2]);
        n       = std::stoul(argv[3]);
    }
    std::cout << "Using file: " << fn << std::endl;


    ModelSchoolQuarantine model(
        n, // Population size
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
        0.9   // Quarantine success rate
    );

    model.read_params(fn, true);
    // model.run(60, 331);

    auto saver = make_save_run<int>(
        out_dir + std::string("/%lu"),
        true, false, false, false, false, true, true, true, false
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

    epiworld::ModelDiagram model_diagram;

    std::vector< std::string > fns;
    for (int i = 0; i < static_cast<int>(model("Replicates")); ++i)
    {
        fns.push_back(
            out_dir + "/" + std::to_string(i) + "_transition.csv"
        );
    }

    model_diagram.draw_from_files(
        fns,
        out_dir + std::string("/diagram.mmd")
    );

    return 0;

}
