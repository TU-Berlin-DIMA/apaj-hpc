#ifndef APAJ_HPC_EXPERIMENT_HPP
#define APAJ_HPC_EXPERIMENT_HPP

#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>
#include <Tables/DimensionTable.hpp>
#include <Tables/FactTable.hpp>

namespace APAJ {

    /**
     * @brief This class represents an experiment with all necessary variables and methods for running one
     */
    class Experiment {

    public:
        static constexpr auto DEFAULT_BATCH_SIZE = 1000UL;
        enum class JOIN_TYPES : int8_t {UNKNOWN = -1, SINGLE = 0, SEQUENTIAL = 1, RANDOM = 2, KNUT_SHUFFLE_L1 = 3,
                                        KNUT_SHUFFLE_L2 = 4, KNUT_SHUFFLE_L3 = 5};

        constexpr static size_t KNUT_SHUFFLE_CYCLE_LENGTHS[4] = {1 * 1024, 12 * 1024, 100 * 1024};

        /**
         * @brief Factory method for creating an experiment from the parameters
         * @param id
         * @param csvFileName
         * @param numTuplesListString
         * @param maxValue
         * @param numReps
         * @param sampleSeq
         * @param selectivitiesListString
         * @param joinTypesString
         * @param numberOfJoins
         * @param withReorderingString
         * @param pathToPapiFldr
         * @param pathToTrainedModel
         * @return Experiment
         */
        static Experiment create(uint64_t id, const std::string& csvFileName, std::string& numTuplesListString,
                                 uint64_t maxValue, uint64_t numReps, const std::string& sampleSeq,
                                 std::string& selectivitiesListString, std::string& joinTypesString,
                                 uint64_t numberOfJoins, const std::string& withReorderingString,
                                 const std::string& pathToPapiFldr, const std::string& pathToTrainedModel);

        /**
         * @brief Creates the data for the allocated tables. This method does not allocate the required memory. It just
         * creates the data for the tables.
         * @param factTable
         * @param firstDimension
         * @param secondDimension
         * @param thirdDimension
         * @param fourthDimension
         */
        void createData(FactTable* factTable, DimensionTable *firstDimension, DimensionTable *secondDimension,
                        DimensionTable *thirdDimension, DimensionTable *fourthDimension) const;

        /**
         * @brief Runs this experiment, e.g., setting up PAPI counters, clearing the caches for each run and performing a n-way join
         * @param startPermutation
         * @param factTable
         * @param firstDimension
         * @param secondDimension
         * @param thirdDimension
         * @param fourthDimension
         */
        void runExperiment(uint64_t startPermutation, FactTable factTable[], DimensionTable firstDimension[],
                           DimensionTable secondDimension[], DimensionTable thirdDimension[],
                           DimensionTable fourthDimension[]) const;

        /**
         * @brief Getter for the id of this experiment
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getId() const;

        /**
         * @brief Getter for the number of tuples in the fact table
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getNumberOfTuplesFactTable() const;

        /**
         * @brief Geter for the number of tuples of the dimension. The dimensions start at 1
         * @param dimension
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getNumberOfTuplesDimension(uint64_t dimension) const;

        /**
         * @brief Getter for the number of permutations
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getNumberOfPermutations() const;

        /**
         * @brief Gets a csv string with all number of tuples for all tables (fact and all dimension tables)
         * @return string
         */
        [[nodiscard]] std::string getAllNumberOfTuplesAsString() const;

        /**
         * @brief Gets a csv string with all join types as strings
         * @return string
         */
        [[nodiscard]] std::string getAllJoinTypesAsString() const;

        /**
         * @brief Gets the join type as a string representation
         * @param types
         * @return string
         */
        static std::string getStringNameJoinType(Experiment::JOIN_TYPES &types);

        /**
         * @brief Creates a string representation of this experiment and returns this as a ostream
         * @param str
         * @param experiment
         * @return ostream
         */
        inline friend std::ostream &operator<<(std::ostream& str, const Experiment &experiment) {
            return str  << "\n- ID: " << std::to_string(experiment.id)
                        << "\n- csvFileName: " << experiment.csvFileName
                        << "\n- numTuples: " << experiment.getAllNumberOfTuplesAsString()
                        << "\n- allJoinTypes: " << experiment.getAllJoinTypesAsString()
                        << "\n- maxValue: " << std::to_string(experiment.maxValue)
                        << "\n- numReps: " << std::to_string(experiment.numReps)
                        << "\n- allSelectivityValues: " << experiment.getAllSelectivitiesCombosAsString()
                        << "\n- withReordering: " << experiment.withReordering
                        << "\n- numberOfJoins: " << experiment.numberOfJoins
                        << "\n- pathToPapiFldr: " << experiment.pathToPapiFldr
                        << "\n- pathToTrainedModel: " << experiment.pathToTrainedModel
                        << "\n- sampleSeq: " << experiment.sampleSeq << "\n";
        }

    private:
        /**
         * @brief Constructor for an experiment
         * @param id
         * @param csvFileName
         * @param numTuples
         * @param maxValue
         * @param numReps
         * @param sampleSeq
         * @param allSelectivityValues
         * @param allJoinTypes
         * @param numberOfJoins
         * @param withReordering
         * @param pathToPapiFldr
         * @param pathToTrainedModel
         */
        Experiment(uint64_t id, std::string csvFileName, std::vector<uint64_t> numTuples, uint64_t maxValue, uint64_t numReps,
                   std::string sampleSeq, std::vector<uint64_t> allSelectivityValues,
                   std::vector<JOIN_TYPES> allJoinTypes, uint64_t numberOfJoins, bool withReordering,
                   std::string pathToPapiFldr, std::string pathToTrainedModel);

        [[nodiscard]] std::string getAllSelectivitiesCombosAsString() const;

        uint64_t id;
        std::string csvFileName;
        std::vector<uint64_t> allNumberOfTuples;
        uint64_t maxValue;
        uint64_t numReps;
        std::string sampleSeq;
        std::vector<uint64_t> allSelectivityValues;
        std::vector<JOIN_TYPES> allJoinTypes;
        uint64_t numberOfJoins;
        bool withReordering;
        std::string pathToPapiFldr;
        std::string pathToTrainedModel;
    };

} // APAJ

#endif //APAJ_HPC_EXPERIMENT_HPP
