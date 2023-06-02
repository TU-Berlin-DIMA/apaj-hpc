#ifndef APAJ_HPC_JOINREORDERING_HPP
#define APAJ_HPC_JOINREORDERING_HPP

#include <Util/Experiment.hpp>
#include <cstdint>
#include <filesystem>
#include <string>

namespace APAJ {

    /**
     * @brief This class encapsulates everything needed for reordering joins, e.g., location of the ml model or the
     * bookkeeping structure for storing the inferred memory access patterns.
     */
    class JoinReordering {
    public:
        /**
         * @brief Constructor for a JoinReordering object
         * @param numberOfJoins
         * @param baseFolder
         */
        JoinReordering(uint64_t numberOfJoins, const std::string& baseFolder);

        /**
         * @brief Retrieves the next permutations. This method expects that the inferred access patterns have been set
         * @param curPermutation
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getNextPermutation(uint64_t curPermutation) const;

        /**
         * @brief Retrieves the path where the model is stored
         * @return path
         */
        [[nodiscard]] std::filesystem::path getModel() const;

        /**
         * @brief Sets the first/outermost memory access pattern / join type for permutation
         * @param curPermutation
         * @param joinType
         */
        void setPermutation(uint64_t curPermutation, Experiment::JOIN_TYPES joinType);

        /**
         * @brief Compares the join types / access patterns of other and current and returns true if other is more favorable
         * @param current
         * @param other
         * @return Boolean
         */
        bool isOtherMoreOptimal(Experiment::JOIN_TYPES** current, Experiment::JOIN_TYPES** other) const;

        /**
         * @brief Deconstructor
         */
        virtual ~JoinReordering();

    private:
        std::filesystem::path baseFolder;
        uint64_t numberOfJoins;
        uint64_t numberOfPermutations;

        Experiment::JOIN_TYPES ***permutationToJoinTypes;
        Experiment::JOIN_TYPES *joinTypesArr;

    };

} // APAJ

#endif //APAJ_HPC_JOINREORDERING_HPP
