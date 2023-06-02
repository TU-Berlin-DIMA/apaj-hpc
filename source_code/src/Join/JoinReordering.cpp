#include <Join/JoinReordering.hpp>
#include <Util/UtilityFunctions.hpp>
#include <algorithm>

namespace APAJ {

    std::filesystem::path JoinReordering::getModel() const {
        return baseFolder;
    }

    JoinReordering::JoinReordering(uint64_t numberOfJoins, const std::string& baseFolder)
        : baseFolder(baseFolder), numberOfJoins(numberOfJoins), numberOfPermutations(Util::factorial(numberOfJoins)) {

        permutationToJoinTypes = (Experiment::JOIN_TYPES***) malloc(numberOfPermutations * sizeof(Experiment::JOIN_TYPES*));
        joinTypesArr = (Experiment::JOIN_TYPES*) malloc(numberOfJoins * sizeof(Experiment::JOIN_TYPES));

        // Initializing joinTypesArr with JOIN_TYPES::UNKNOWN
        std::vector<uint64_t> permutationVec;
        for (auto i = 0UL; i < numberOfJoins; ++i) {
            joinTypesArr[i] = Experiment::JOIN_TYPES::UNKNOWN;
            permutationVec.emplace_back(i);
        }

        // Go through all permutations and direct pointer from permutationToJoinTypes[curPermutation][i] to joinTypesArr[i]
        uint64_t curPerm = 0;
        do {
            permutationToJoinTypes[curPerm] = (Experiment::JOIN_TYPES**) malloc(numberOfJoins * sizeof(Experiment::JOIN_TYPES*));
            for (size_t i = 0; i < numberOfJoins; ++i) {
                this->permutationToJoinTypes[curPerm][i] = &joinTypesArr[permutationVec[i]];
            }
            ++curPerm;
        } while(std::next_permutation(permutationVec.begin(), permutationVec.end()));
    }

    uint64_t JoinReordering::getNextPermutation(uint64_t curPermutation) const {
        auto nextPerm = curPermutation;
        for (auto perm = 0UL; perm < numberOfPermutations; ++perm) {
            if (isOtherMoreOptimal(permutationToJoinTypes[nextPerm], permutationToJoinTypes[perm])) {
                nextPerm = perm;
            }
        }

        return nextPerm;
    }

    void JoinReordering::setPermutation(uint64_t curPermutation, Experiment::JOIN_TYPES joinType) {
        *permutationToJoinTypes[curPermutation][0] = joinType;
    }

    bool JoinReordering::isOtherMoreOptimal(Experiment::JOIN_TYPES **current, Experiment::JOIN_TYPES **other) const {
        // Compare who has RANDOM joins at the latest stage
        for (auto join = 0UL; join < numberOfJoins; ++join) {
            if (*current[join] == Experiment::JOIN_TYPES::RANDOM && *other[join] != Experiment::JOIN_TYPES::RANDOM) {
                return true;
            }
            if (*current[join] != Experiment::JOIN_TYPES::RANDOM && *other[join] == Experiment::JOIN_TYPES::RANDOM) {
                return false;
            }
        }

        return false;
    }

    JoinReordering::~JoinReordering() {
        for (auto perm = 0UL; perm < numberOfPermutations; ++perm) {
            free(permutationToJoinTypes[perm]);
        }

        free(permutationToJoinTypes);
        free(joinTypesArr);
    }

} // namespace APAJ