#ifndef APAJ_HPC_UTILITYFUNCTIONS_HPP
#define APAJ_HPC_UTILITYFUNCTIONS_HPP

#include <Tables/FactTable.hpp>

#include <string>
#include <vector>

namespace APAJ::Util {

    /**
     * @brief calculates n!
     * @param n
     * @return uint64_t
     */
    constexpr inline uint64_t factorial(uint64_t n) {
        if (n <= 1) {
            return 1;
        }
        return n * factorial(n - 1);
    }

    /**
     * @brief Clears all caches by writing dummy values randomly
     * @param maxCacheSize
     * @param dummyArray1
     * @param dummyArray2
     * @return size_t
     */
    size_t clearCaches(size_t maxCacheSize, const int *dummyArray1, const int *dummyArray2);

    /**
     * @brief Initializes the three dummy arrays for clearing the cache
     * @param maxCacheSize
     * @param dummyArray1
     * @param dummyArray2
     * @param dummyArray3
     */
    void initCacheClear(size_t maxCacheSize, int *dummyArray1, int *dummyArray2, int *dummyArray3);

    /**
     * @param factTable
     * @param numberOfTuplesJoin
     * @param start
     * @param cycleLength
     * @param joinNumber is which join is to done
     * @brief creates data in a knut shuffle way for factTable in the range of [start, start + cycleLength] with values of [start, start + cycleLength]
     */
    void createDataKnutShuffle(FactTable* factTable, uint64_t numberOfTuplesJoin, uint64_t start, uint64_t cycleLength,
                               uint64_t joinNumber);

    /**
         * @param min
         * @param max
         * @return next integer in range of [min, max]
         */
    inline uint64_t getNextInteger(uint64_t min, uint64_t max);

    /**
     * @brief Parsing string in form of [\d+, \d+, \d+, \d+, \d+] for the number of tuples for each table
     * @param string
     * @return std::vector<size_t>
     */
    std::vector<double> parseStringToDoubleVec(std::string& string);

    /**
     * @brief Parsing string in form of [\d+, \d+, \d+, \d+, \d+] for the number of tuples for each table
     * @param string
     * @return std::vector<size_t>
     */
    std::vector<size_t> parseStringToUIntVec(std::string& string);

} // namespace APAJ

#endif //APAJ_HPC_UTILITYFUNCTIONS_HPP
