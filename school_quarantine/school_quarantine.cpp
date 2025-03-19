// #define EPI_DEBUG
#include "school_quarantine.hpp"

// Defining the 

int main(int argc, char *argv[]) {

    // Passing the single argument to this function
    std::string fn      = "new_bridge_88_params.yaml";
    std::string out_dir = "new_bridge_88";
    if (argc != 3 && argc != 1) {
        std::cerr << "Usage: " << argv[0] << " <filename> <outdir>" << std::endl;
        return 1;
    } else if (argc == 4) {
        fn      = std::string(argv[1]);
        out_dir = std::string(argv[2]);
    }
    std::cout << "Using file: " << fn << std::endl;

    // Reading parameters from the model
    auto params = epiworld::read_yaml<epiworld_double>(fn);

    ModelSchoolQuarantine model(
        (size_t) params["Population size"],                 
        (int) params["initial number of exposed"], 
        params["Contact rate"],   // Contact rate  
        params["Transmission rate"], // Transmission rate
        params["Vax efficacy"], // Vaccine reduction in susceptibility
        params["Vax improved recovery"],  // Vaccine reduction in recovery rate
        params["Incubation period"],    // Incubation period
        params["Prodromal period"],    // Prodromal period
        params["Rash period"],    // Rash period
        params["Days undetected"],    // Days undetected
        params["Hospitalization rate"],    // Hospitalization rate
        params["Hospitalization days"],
        params["Vaccination rate"],   // Vaccination rate
        params["Quarantine days"],   // Quarantine days for vaccinated
        params["Quarantine willingness"]   // Quarantine success rate
    );

    // model.read_params(fn, true);
    // model.run(60, 331);

    auto saver = make_save_run<int>(
        out_dir + std::string("/%lu"),
        true, false, false, false, false, true, true, true, false
    );

    model.run_multiple(
        static_cast<int>(params["N days"]), // Number of days
        static_cast<int>(params["Replicates"]), // Number of sims
        static_cast<int>(params["Seed"]), // Seed
        saver,
        true,
        true,
        static_cast<int>(params["Threads"])
    );

    model.print();

    epiworld::ModelDiagram model_diagram;

    std::vector< std::string > fns;
    for (int i = 0; i < static_cast<int>(params["Replicates"]); ++i)
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
