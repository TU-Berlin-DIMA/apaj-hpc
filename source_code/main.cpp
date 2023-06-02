
#include <Tables/FactTable.hpp>
#include <Tables/DimensionTable.hpp>
#include <Util/APAJLogger.hpp>
#include <iostream>
#include <omp.h>
#include <Util/Experiment.hpp>


int main(int argc, char* argv[]) {
    using namespace APAJ;

    if (argc != 12) {
        std::cout << argv[0] << " <EXPERIMENT ID> <CSV FILENAME> <NUMBER OF TUPLES Per Table as list e.g. [1e10, 5 * 1e10, ...]>"
                  "<MAX_VALUE> <NUM REPS PER EXPERIMENT> <SAMPLE SEQUENCE for HW Counters>"
                  "<Selectivities as list e.g. [0.5, 0.2, ...]> <Joint Types Per Table as list e.g. [2, 1, 1, ...]>"
                  " <REORDERING ENABLED OR DISABLED either True or False> <Path to PAPI config file> <Path to trained ML model>\n";
        std::cout << "Wrong parameter count!!!" << std::endl;
        exit(-1);
    }

    APAJ::APAJLogger->removeAllAppenders();
    APAJ::setupLogging("main.log", DebugLevel::LOG_INFO);

    // Setting the CPU affinity to make sure that this process never switches the core
    uint64_t processor = 0;
    cpu_set_t  mask;
    CPU_ZERO(&mask);
    CPU_SET(processor, &mask);
    int result = sched_setaffinity(0, sizeof(mask), &mask);
    if (result != 0) {
        throw RuntimeException("Could not set sched_affinity to core 0!!!");
    }
    APAJ_INFO("Running on processor: " << processor);

	uint64_t NUMBER_OF_JOINS = 3;

    auto numTuplesListString = std::string(argv[3]);
    auto selectivitiesListString = std::string(argv[7]);
    auto joinTypesString = std::string(argv[8]);
    auto experiment = Experiment::create(atoll(argv[1]), argv[2], numTuplesListString, atoll(argv[4]),
                                         atoll(argv[5]), argv[6], selectivitiesListString, joinTypesString,
                                         NUMBER_OF_JOINS, argv[9], argv[10], argv[11]);

    // Allocating the required memory for this experiment
    std::vector<FactTable> factTable(experiment.getNumberOfTuplesFactTable());
    std::vector<DimensionTable> firstDimension(experiment.getNumberOfTuplesDimension(1));
    std::vector<DimensionTable> secondDimension(experiment.getNumberOfTuplesDimension(2));
    std::vector<DimensionTable> thirdDimension(experiment.getNumberOfTuplesDimension(3));
    std::vector<DimensionTable> fourthDimension(experiment.getNumberOfTuplesDimension(4));

    APAJ_INFO("Creating data according to: " << experiment.getAllJoinTypesAsString());
    experiment.createData(factTable.data(), firstDimension.data(), secondDimension.data(), thirdDimension.data(),
                          fourthDimension.data());
    APAJ_INFO("Experiment data has been created!");

    APAJ_INFO("Experiment params: \n" << experiment);
    auto numberOfPermutations = experiment.getNumberOfPermutations();
    for (auto perm = 0UL; perm < numberOfPermutations; ++perm) {
        APAJ_INFO("Running permutation: " << (perm + 1) << "/" << numberOfPermutations);
        experiment.runExperiment(perm, factTable.data(), firstDimension.data(), secondDimension.data(),
                                 thirdDimension.data(), fourthDimension.data());
    }

    APAJ_INFO("Experiment " << experiment.getId() << " has finished!\n\n");


    return 0;
}