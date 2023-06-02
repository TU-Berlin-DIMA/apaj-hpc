#ifndef APAJ_HPC_PAPI_HELPER_HPP
#define APAJ_HPC_PAPI_HELPER_HPP

#include <papi.h>
#include <string>
#include <vector>
#include <filesystem>

namespace APAJ::PAPI {

    /**
     * @brief Struct that stores all information related to communicating with PAPI
     */
    struct sampling {
        std::vector<std::string> counterNames;
        int EventSet;
        std::vector<long_long> result;
        std::vector<long_long> batchResult;
        size_t numberOfCounters;
        size_t numberOfTuples;
        std::string sampleSeq = "TBD";
    };

    /**
     * @brief Initializes all PPAI related stuff, e.g., adds the counters and sets the sampleStruct up to be able to
     * retrieve hardware performance counter
     * @param sampleStruct
     * @param pathToPapiFldr
     */
    void initPapi(sampling& sampleStruct, const std::filesystem::path& pathToPapiFldr);

    /**
     * @brief Parses the counters from the papi file papiCfgFile and creates a vector out of strings
     * @param sampleStruct
     * @param pathToPapiFldr
     * @param papiCfgFile
     * @return Vector of strings
     */
    std::vector<std::string> readCountersFromPAPICfgFile(sampling& sampleStruct, const std::filesystem::path& pathToPapiFldr,
                                                         std::string papiCfgFile = "papi_conf_global.cfg");

    /**
     * @brief Retrieves a csv string from the papi file papiCfgFile
     * @param sampleStruct
     * @param pathToPapiFldr
     * @return ostringstream
     */
    std::ostringstream getCSVHeader(sampling& sampleStruct, const std::filesystem::path& pathToPapiFldr);

    /**
     * @brief Logs the error and then exits the program
     * @param retVal
     * @param msg
     */
    void papiHandleError(int retVal, std::string msg);

} // namespace APAJ::PAPI

#endif //APAJ_HPC_PAPI_HELPER_HPP
