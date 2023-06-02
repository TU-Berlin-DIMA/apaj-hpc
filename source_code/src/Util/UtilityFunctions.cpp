#include <Util/UtilityFunctions.hpp>

#include <algorithm>
#include <vector>
#include <sstream>

namespace APAJ::Util {

    std::vector<double> parseStringToDoubleVec(std::string& string) {
        std::vector<double> retVec;

        string.erase(std::remove(string.begin(), string.end(), '['), string.end());
        string.erase(std::remove(string.begin(), string.end(), ']'), string.end());

        std::istringstream stringStreamNumberOfTuples(string);
        std::string token;
        while(std::getline(stringStreamNumberOfTuples, token, ',')) {
            auto numTuples = std::stod(token);
            retVec.emplace_back(numTuples);
        }

        return retVec;
    }

    std::vector<size_t> parseStringToUIntVec(std::string& string) {
        std::vector<size_t> retVec;

        string.erase(std::remove(string.begin(), string.end(), '['), string.end());
        string.erase(std::remove(string.begin(), string.end(), ']'), string.end());

        std::istringstream stringStreamNumberOfTuples(string);
        std::string token;
        while(std::getline(stringStreamNumberOfTuples, token, ',')) {
            auto numTuples = std::stoll(token);
            retVec.emplace_back(numTuples);
        }

        return retVec;
    }

    void initCacheClear(uint64_t maxCacheSize, int *dummyArray1, int *dummyArray2, int *dummyArray3) {
        for (auto address = 0UL; address < maxCacheSize; address++) {
            dummyArray1[address] = address + 1;
        }

        for (auto address = 0UL; address < maxCacheSize; address++) {
            dummyArray2[address] = address + 128;
        }

        for (auto address = 0UL; address < maxCacheSize; address++) {
            dummyArray3[address] = address + 7829;
        }
    }

    uint64_t  clearCaches(uint64_t maxCacheSize, const int *dummyArray1, const int *dummyArray2) {
        auto sum = 0UL;
        for (auto address = 0UL; address < maxCacheSize; address++) {
            sum += address + 1;
        }

        for (auto address = 0UL; address < maxCacheSize; address += 2) {
            sum += address + 128;
        }
        for (auto address = 0UL; address < maxCacheSize * 4; address += 4) {
            sum += address + 7829;
        }

        for (int repetition = 0; repetition < 3; repetition++) {
            for (auto address = 0UL; address < maxCacheSize; address++) {
                sum += dummyArray1[address];
                sum += dummyArray2[address];
            }
        }

        return sum;
    }

    void createDataKnutShuffle(FactTable *factTable, uint64_t numberOfTuplesJoin, uint64_t start, uint64_t cycleLength,
                               uint64_t joinNumber) {
        APAJ_INFO("Util::createDataKnutShuffle for joinNumber = " << joinNumber << " start = " << start
                  << " cycleLength = " << cycleLength << " numberOfTuplesJoin = " << numberOfTuplesJoin);

        for (auto i = 0UL; i < cycleLength; ++i) {
            auto j = getNextInteger(0, i);
            if ( j != i) {
                factTable[start + i].setDimensionKey(factTable[start + j].getDimensionKey(joinNumber), joinNumber);
            }
            factTable[start + j].setDimensionKey((start + i) % numberOfTuplesJoin, joinNumber);
        }

    }

    uint64_t getNextInteger(uint64_t min, uint64_t max) {
        return  min + rand() / (RAND_MAX / (max - min + 1) + 1);
    }

} // namespace APAJ

