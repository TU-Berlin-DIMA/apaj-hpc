#include <Join/JoinReordering.hpp>
#include <Join/join_permutations.hpp>
#include <Util/Experiment.hpp>
#include <Util/UtilityFunctions.hpp>
#include <Papi/papi_helper.hpp>
#include <cppflow/include/cppflow/ops.h>
#include <cppflow/include/cppflow/model.h>

#include <algorithm>
#include <numeric>
#include <omp.h>
#include <papi.h>
#include <random>
#include <string>
#include <utility>

namespace APAJ {

    Experiment Experiment::create(size_t id, const std::string& csvFileName, std::string& numTuplesListString,
                                  size_t maxValue, size_t numReps, const std::string& sampleSeq,
                                  std::string& allSelectivitiesListString, std::string& allJoinTypesString,
                                  size_t numberOfJoins, const std::string& withReorderingString,
                                  const std::string& pathToPapiFldr, const std::string& pathToTrainedModel) {

        // Parsing string to the number of tuples for each table
        auto allNumberOfTuples = Util::parseStringToUIntVec(numTuplesListString);

        // Now we do the same for the selectivity
        std::vector<uint64_t> allSelectivityValues;
        for (const auto& sel : Util::parseStringToDoubleVec(allSelectivitiesListString)) {
            allSelectivityValues.emplace_back(sel * maxValue);
        }

        // And again for the join types
        std::vector<JOIN_TYPES> allJoinTypes;
        for (auto& joinInt : Util::parseStringToUIntVec(allJoinTypesString)) {
            switch(joinInt) {
                case static_cast<size_t>(JOIN_TYPES::SINGLE): allJoinTypes.emplace_back(JOIN_TYPES::SINGLE); break;
                case static_cast<size_t>(JOIN_TYPES::SEQUENTIAL): allJoinTypes.emplace_back(JOIN_TYPES::SEQUENTIAL); break;
                case static_cast<size_t>(JOIN_TYPES::RANDOM): allJoinTypes.emplace_back(JOIN_TYPES::RANDOM); break;
                case static_cast<size_t>(JOIN_TYPES::KNUT_SHUFFLE_L1): allJoinTypes.emplace_back(JOIN_TYPES::KNUT_SHUFFLE_L1); break;
                case static_cast<size_t>(JOIN_TYPES::KNUT_SHUFFLE_L2): allJoinTypes.emplace_back(JOIN_TYPES::KNUT_SHUFFLE_L2); break;
                case static_cast<size_t>(JOIN_TYPES::KNUT_SHUFFLE_L3): allJoinTypes.emplace_back(JOIN_TYPES::KNUT_SHUFFLE_L3); break;
                default: throw std::runtime_error(std::string("joinType ") + std::to_string(joinInt) + std::string(" is undefined"));
            }
        }

        // Checking if withReorderingString is true or false
        bool withReordering;
        if (withReorderingString == "True" || withReorderingString == "true") {
            withReordering = true;
        } else if (withReorderingString == "False" || withReorderingString == "false") {
            withReordering = false;
        } else {
            throw std::runtime_error(std::string("withReorderingString: ") + withReorderingString +
                                     std::string(" is neither true or false!"));
        }

        return {id, csvFileName, allNumberOfTuples, maxValue, numReps, sampleSeq, allSelectivityValues, allJoinTypes,
                numberOfJoins, withReordering, pathToPapiFldr, pathToTrainedModel};

    }

    size_t Experiment::getNumberOfPermutations() const {
        return Util::factorial(numberOfJoins);
    }

    size_t Experiment::getNumberOfTuplesFactTable() const {
        return allNumberOfTuples[0];
    }

    size_t Experiment::getNumberOfTuplesDimension(uint64_t dimension) const {
        if (dimension == 0) {
            APAJ_ERROR("Dimension 0 doe snot exist. Do you maybe mean dimension 1?");
            std::runtime_error("Wrong dimension!!!");
        }
        return allNumberOfTuples[dimension];
    }

    std::string Experiment::getAllNumberOfTuplesAsString() const {
        std::ostringstream ossVector;
        for(size_t i = 0; i < allNumberOfTuples.size(); i++) {
            if (i != 0) {
                ossVector << ", ";
            }
            ossVector << allNumberOfTuples[i];
        }
        return ossVector.str();
    }

    std::string Experiment::getAllJoinTypesAsString() const {
        std::ostringstream ossVector;
        for(size_t i = 0; i < allJoinTypes.size(); i++) {
            if (i != 0) {
                ossVector << ", ";
            }

            auto joinType = allJoinTypes[i];
            ossVector << getStringNameJoinType(joinType);
        }
        return ossVector.str();
    }

    std::string Experiment::getStringNameJoinType(Experiment::JOIN_TYPES &types) {
        switch (types) {
            case JOIN_TYPES::SINGLE: return "SINGLE";
            case JOIN_TYPES::SEQUENTIAL: return "SEQUENTIAL";
            case JOIN_TYPES::RANDOM: return "RANDOM";
            case JOIN_TYPES::KNUT_SHUFFLE_L1: return "KNUT_SHUFFLE_L1";
            case JOIN_TYPES::KNUT_SHUFFLE_L2: return "KNUT_SHUFFLE_L2";
            case JOIN_TYPES::KNUT_SHUFFLE_L3: return "KNUT_SHUFFLE_L3";
            case JOIN_TYPES::UNKNOWN: return "UNKNOWN";
            default: return "NOT FOUND!!!";
        }
    }

    std::string Experiment::getAllSelectivitiesCombosAsString() const {
        std::ostringstream ossVector;
        for(size_t i = 0; i < allSelectivityValues.size(); i++) {
            if (i != 0) {
                ossVector << ", ";
            }
            ossVector << (static_cast<double>(allSelectivityValues[i]) / static_cast<double>(maxValue));
        }
        return ossVector.str();
    }

    void Experiment::runExperiment(uint64_t startPermutation, FactTable* factTable, DimensionTable* firstDimension,
                                   DimensionTable* secondDimension, DimensionTable* thirdDimension,
                                   DimensionTable* fourthDimension) const {

        // So that no compilation errors occur, if not all tables are used
        ((void) factTable);
        ((void) firstDimension);
        ((void) secondDimension);
        ((void) thirdDimension);
        ((void) fourthDimension);

        // Initializing all PAPI stuff
        PAPI::sampling sampleStruct;
        sampleStruct.sampleSeq = sampleSeq;
        PAPI::initPapi(sampleStruct, pathToPapiFldr);

        // Allocating dummy arrays for clearing the caches
        size_t CACHE_SIZE_IN_MB = 32;
        size_t maxCacheSize = 1024 * 1024 * CACHE_SIZE_IN_MB * 16;
        std::vector<int> dummyArray1(maxCacheSize, 0);
        std::vector<int> dummyArray2(maxCacheSize, 0);
        std::vector<int> dummyArray3(maxCacheSize, 0);

        // Allocating variables for all reps
        uint64_t dummyVar = 0;
        uint64_t duration = 0;
        std::vector<uint64_t> hwCountersPerExperiment(sampleStruct.numberOfCounters, 0);
        std::unique_ptr<cppflow::model> model;

        for (uint64_t rep = 0; rep < numReps; ++rep) {
            auto curPermutation = startPermutation;
            auto BATCH_SIZE = withReordering ? DEFAULT_BATCH_SIZE : this->getNumberOfTuplesFactTable();
            APAJ_DEBUG("BATCH_SIZE: " << BATCH_SIZE);

            APAJ_DEBUG("Creating tensorflow model and joinReordering...");
            JoinReordering joinReordering(numberOfJoins, pathToTrainedModel);
            if (withReordering) {
                model = std::make_unique<cppflow::model>(joinReordering.getModel());
            }
            APAJ_DEBUG("Done creating tensorflow model and joinReordering from " << (joinReordering.getModel()));

            sampleStruct.numberOfTuples = 0;
            for (uint u = 0; u < sampleStruct.numberOfCounters; ++u) {
                sampleStruct.result[u] = 0;
                sampleStruct.batchResult[u] = 0;
            }

            APAJ_DEBUG("Clearing the caches...");
            Util::initCacheClear(maxCacheSize, dummyArray1.data(), dummyArray2.data(), dummyArray3.data());
            Util::clearCaches(maxCacheSize, dummyArray1.data(), dummyArray2.data());
            APAJ_DEBUG("Done clearing the caches!");

            int retVal;
            auto timesPredicted = 0UL;
            auto startTs = std::chrono::high_resolution_clock::now();
            for (auto curBatchPos = 0UL; curBatchPos < this->getNumberOfTuplesFactTable(); curBatchPos += BATCH_SIZE) {
                if (timesPredicted >= numberOfJoins) {
                    BATCH_SIZE = this->getNumberOfTuplesFactTable() - curBatchPos;
                }

                auto startOffset = curBatchPos;
                auto stopOffset = (startOffset + BATCH_SIZE) < this->getNumberOfTuplesFactTable() ? (startOffset + BATCH_SIZE) : this->getNumberOfTuplesFactTable();
                APAJ_DEBUG("Experiment: startOffset = " << startOffset << " stopOffset = " << stopOffset);

                // Starting hw counters
                if ((retVal = PAPI_start(sampleStruct.EventSet)) != PAPI_OK) {
                    PAPI::papiHandleError(retVal, "Error starting PAPI");
                }

                // Performing the join for the current permutation
                dummyVar = PermutationJoin::permutationJoin(curPermutation, dummyVar, startOffset, stopOffset,
                                                            allSelectivityValues, factTable, firstDimension,
                                                            secondDimension, thirdDimension, fourthDimension);

                // Stopping hw counters
                std::vector<long_long> resultCounter(sampleStruct.numberOfCounters, 0);
                if ((retVal = PAPI_stop(sampleStruct.EventSet, resultCounter.data())) != PAPI_OK) {
                    PAPI::papiHandleError(retVal, "Error stopping PAPI");
                }

                // Storing the hw counter values in the sampleStruct
                for (auto u = 0UL; u < sampleStruct.numberOfCounters; ++u) {
                    sampleStruct.result[u] += resultCounter[u];
                    sampleStruct.batchResult[u] += resultCounter[u];
                }
                sampleStruct.numberOfTuples += (stopOffset - startOffset);
            }

            if (withReordering && numberOfJoins > timesPredicted) {
                auto startPredictionTime = std::chrono::high_resolution_clock::now();

                std::vector<float> inputVector(sampleStruct.numberOfCounters);
                for (auto u = 0UL; u < sampleStruct.numberOfCounters; ++u) {
                    // Normalizing hardware performance counters
                    inputVector[u] = (float) sampleStruct.batchResult[u] / ((float) sampleStruct.numberOfTuples);
                }

                const std::vector<int64_t> inputTensorShape({1, 3});
                const cppflow::tensor input(inputVector, inputTensorShape);
                auto output = (*model)(input);

                bool swapPermutation = (output.get_data<float>()[0]) > 0.5;
                auto endPredictionTime = std::chrono::high_resolution_clock::now();
                auto timeDifference = std::chrono::duration_cast<std::chrono::microseconds>(endPredictionTime - startPredictionTime).count();

                APAJ_INFO("runExperiment: model prediction takes " << timeDifference << " us!");

                if (swapPermutation) {
                    joinReordering.setPermutation(curPermutation, Experiment::JOIN_TYPES::RANDOM);
                    curPermutation = joinReordering.getNextPermutation(curPermutation);
                    APAJ_INFO("runExperiment: curPermutation = " << curPermutation);
                } else {
                    joinReordering.setPermutation(curPermutation, Experiment::JOIN_TYPES::SINGLE);
                    APAJ_INFO("Staying in current permutation " << curPermutation);
                }
                ++timesPredicted;
            }

            auto endTs = std::chrono::high_resolution_clock::now();
            duration += std::chrono::duration_cast<std::chrono::milliseconds>(endTs - startTs).count();

            for (auto i = 0UL; i < sampleStruct.numberOfCounters; ++i) {
                hwCountersPerExperiment[i] += sampleStruct.result[i];
            }
            APAJ_INFO("Done with rep: " << (rep + 1) << " of " << numReps);
        }

        double avgExecTime = (double) duration / (double) numReps;
        std::ostringstream hwCounterStream;
        std::ostringstream hwCounterNames;
        for (auto i = 0UL; i < sampleStruct.numberOfCounters; ++i) {
            if (i != 0) {
                hwCounterStream << ",";
                hwCounterNames << ",";
            }

            hwCountersPerExperiment[i] /= numReps;
            hwCounterStream << std::to_string(hwCountersPerExperiment[i]);
            hwCounterNames << sampleStruct.counterNames[i];
        }

        APAJ_INFO("Experiment took on average " << avgExecTime << " ms with " << numReps << " reps!");
        APAJ_INFO("HW counters: " << hwCounterStream.str());
        APAJ_INFO("Selectivities: " << this->getAllSelectivitiesCombosAsString());
        APAJ_INFO("Join types: " << this->getAllJoinTypesAsString());

        std::vector<uint64_t> v(allSelectivityValues.size());
        std::iota (std::begin(v), std::end(v), 0);
        std::ostringstream ossSelectivitiesName;
        ossSelectivitiesName << "Selectivity_";
        std::copy(v.begin(), v.end()-1,std::ostream_iterator<int>(ossSelectivitiesName, ",Selectivity_"));
        ossSelectivitiesName << v.back();

        /* Writing to csv file */
        std::ofstream csvFile;
        csvFile.open(csvFileName, std::ios_base::app);
        csvFile << "EXPERIMENT_ID,EXEC_TIME_MS,PERMUTATION," << hwCounterNames.str() << "," << ossSelectivitiesName.str() << "\n";
        csvFile << this->getId() << "," << avgExecTime << "," << std::to_string(startPermutation) << "," << hwCounterStream.str() << ","
                << this->getAllSelectivitiesCombosAsString() << "\n";

        csvFile.close();
    }

    uint64_t Experiment::getId() const {
        return id;
    }

    Experiment::Experiment(uint64_t id, std::string csvFileName, std::vector<uint64_t> numTuples, uint64_t maxValue,
                           uint64_t numReps, std::string sampleSeq, std::vector<uint64_t> allSelectivityValues,
                           std::vector<JOIN_TYPES> allJoinTypes, uint64_t numberOfJoins, bool withReordering,
                           std::string pathToPapiFldr, std::string pathToTrainedModel)
                           : id(id), csvFileName(std::move(csvFileName)), allNumberOfTuples(std::move(numTuples)), maxValue(maxValue),
                           numReps(numReps), sampleSeq(std::move(sampleSeq)), allSelectivityValues(std::move(allSelectivityValues)),
                           allJoinTypes(std::move(allJoinTypes)), numberOfJoins(numberOfJoins), withReordering(withReordering),
                           pathToPapiFldr(std::move(pathToPapiFldr)), pathToTrainedModel(std::move(pathToTrainedModel)) {}

    void Experiment::createData(FactTable *factTable, DimensionTable *firstDimension, DimensionTable *secondDimension,
                                DimensionTable *thirdDimension, DimensionTable *fourthDimension) const {

        // Uniform key
        std::random_device random_device_key;
        std::mt19937 generatorKey(random_device_key());
        std::uniform_int_distribution<> distribKey(0, static_cast<int>(getNumberOfTuplesFactTable() - 1));

        // Uniform Value
        std::random_device random_device_value;
        std::mt19937 generatorValue(random_device_value());
        std::uniform_int_distribution<> distribValue(0, static_cast<int>(maxValue - 1));

        for (auto i = 0UL; i < getNumberOfTuplesFactTable(); ++i) {
            std::vector<uint64_t> dimensionKeys;
            for (auto& joinType : allJoinTypes) {
                uint64_t tmpKey;

                switch (joinType) {
                    case JOIN_TYPES::UNKNOWN: APAJ_ERROR("Trying to create data for unknown join type!"); break;

                    case JOIN_TYPES::SINGLE: tmpKey = 1; break;

                    case JOIN_TYPES::SEQUENTIAL:
                    case JOIN_TYPES::KNUT_SHUFFLE_L1:
                    case JOIN_TYPES::KNUT_SHUFFLE_L2:
                    case JOIN_TYPES::KNUT_SHUFFLE_L3: tmpKey = i; break;

                    case JOIN_TYPES::RANDOM: tmpKey = distribKey(generatorKey); break;
                }

                dimensionKeys.emplace_back(tmpKey);
            }

            uint64_t lineOrderValue = distribValue(generatorValue);
            factTable[i] = FactTable(i, lineOrderValue,
                                     {dimensionKeys[0] % getNumberOfTuplesDimension(1),
                                     dimensionKeys[1] % getNumberOfTuplesDimension(2),
                                     dimensionKeys[2] % getNumberOfTuplesDimension(3),
                                     dimensionKeys[3] % getNumberOfTuplesDimension(4)});
        }

        // Init KnutShuffle versions
        for(auto joinNumber = 0UL; joinNumber < allJoinTypes.size(); ++joinNumber) {

            auto& joinType = allJoinTypes[joinNumber];
            auto cycleLength = 0UL;
            switch(joinType) {
                case JOIN_TYPES::UNKNOWN:
                case JOIN_TYPES::SINGLE:
                case JOIN_TYPES::SEQUENTIAL:
                case JOIN_TYPES::RANDOM: APAJ_INFO("Join type is not any KnutShuffle version"); continue;

                case JOIN_TYPES::KNUT_SHUFFLE_L1: cycleLength = KNUT_SHUFFLE_CYCLE_LENGTHS[0]; break;
                case JOIN_TYPES::KNUT_SHUFFLE_L2: cycleLength = KNUT_SHUFFLE_CYCLE_LENGTHS[1]; break;
                case JOIN_TYPES::KNUT_SHUFFLE_L3: cycleLength = KNUT_SHUFFLE_CYCLE_LENGTHS[2]; break;
            }

            for (size_t startPos = 0; startPos < allNumberOfTuples[joinNumber + 1]; startPos += cycleLength) {
                if (startPos + cycleLength >= allNumberOfTuples[joinNumber + 1]) {
                    cycleLength = allNumberOfTuples[joinNumber] - startPos;
                }
                Util::createDataKnutShuffle(factTable, allNumberOfTuples[joinNumber + 1], startPos, cycleLength,
                                            joinNumber + 1);
            }
        }

        for (auto i = 0UL; i < getNumberOfTuplesDimension(1); ++i) {
            firstDimension[i] = DimensionTable(i, distribValue(generatorValue));
        }

        for (auto i = 0UL; i < getNumberOfTuplesDimension(2); ++i) {
            secondDimension[i] = DimensionTable(i, distribValue(generatorValue));
        }

        for (auto i = 0UL; i < getNumberOfTuplesDimension(3); ++i) {
            thirdDimension[i] = DimensionTable(i, distribValue(generatorValue));
        }

        for (auto i = 0UL; i < getNumberOfTuplesDimension(4); ++i) {
            fourthDimension[i] = DimensionTable(i, distribValue(generatorValue));
        }
    }
} // APAJ