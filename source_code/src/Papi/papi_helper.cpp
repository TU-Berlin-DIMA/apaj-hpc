
#include <Papi/papi_helper.hpp>
#include <string>
#include <vector>
#include <Util/APAJLogger.hpp>
#include <filesystem>
#include <fstream>
#include <cassert>

namespace APAJ::PAPI {

    void initPapi(sampling &sampleStruct, const std::filesystem::path& pathToPapiFldr) {
        if (sampleStruct.sampleSeq == "time") {
            APAJ_INFO("Skip papi init for time");
            return;
        }

        // Initialize event
        sampleStruct.counterNames = readCountersFromPAPICfgFile(sampleStruct, pathToPapiFldr);
        sampleStruct.EventSet = PAPI_NULL;
        sampleStruct.numberOfCounters = sampleStruct.counterNames.size();

        // Initialize the library
        int retVal = PAPI_library_init(PAPI_VER_CURRENT);
        if (retVal != PAPI_VER_CURRENT) {
            printf("PAPI library init error!\n");
            exit(1);
        }

        if (PAPI_thread_init(pthread_self) != PAPI_OK) {
            papiHandleError(1, "PAPI thread init error!\n");
        }

        if ((retVal = PAPI_create_eventset(&sampleStruct.EventSet)) != PAPI_OK) {
            papiHandleError(retVal, "Can not create event set!");
        }

        // Insert PAPI counters
        int eventCode;
        PAPI_event_info_t info;
        sampleStruct.result.clear();
        for (auto& it : sampleStruct.counterNames) {
            APAJ_INFO("Trying to parse: " << it);
            char* tempCode = const_cast<char*>(it.c_str());

            // Getting the event code from the event name
            retVal = PAPI_event_name_to_code(tempCode, &eventCode);
            if (retVal != PAPI_OK) {
                papiHandleError(retVal, "Error converting event <" + it + ">");
            }

            // Getting the event info from the code
            retVal = PAPI_get_event_info(eventCode, &info);
            if (retVal != PAPI_OK) {
                papiHandleError(retVal, "Error for event info <" + it + ">");
            }

            // Checking if the event is available
            retVal = PAPI_query_event(eventCode);
            if (retVal != PAPI_OK) {
                papiHandleError(retVal, "No counter: " + std::string(info.symbol));
            }

            // Adding the event
            retVal = PAPI_add_event(sampleStruct.EventSet, eventCode);
            if (retVal != PAPI_OK) {
                papiHandleError(retVal, "Error adding event <" + it + ">");
            }

            sampleStruct.result.emplace_back(0);
            sampleStruct.batchResult.emplace_back(0);
        }
    }

    std::vector<std::string> readCountersFromPAPICfgFile(sampling& sampleStruct, const std::filesystem::path& pathToPapiFldr,
                                                         std::string papiCfgFile) {
        std::vector<std::string> counter;
        papiCfgFile = pathToPapiFldr / "papi_conf_global.cfg";
        APAJ_INFO("Using papi config file: " << papiCfgFile);

        std::ifstream infile(papiCfgFile.c_str());
        if (!infile.is_open()) {
            APAJ_ERROR("papi_helper: Could not open " << papiCfgFile.c_str());
            exit(1);
        }

        // Find line in file
        std::string line, values;
        std::string counterSeq = sampleStruct.sampleSeq + "=";
        while(std::getline(infile, line)) {
            std::istringstream iss(line);
            if (line.substr(0, counterSeq.size()) == counterSeq) {
                auto pos = line.find('=');
                values = line.substr(pos + 1, line.length());
            }
        }

        // Parsing csv into a vector of strings
        auto pos = 0UL;
        std::string delimiter = ",";
        std::string token;
        while ((pos = values.find(delimiter)) != std::string::npos) {
            token = values.substr(0, pos);
            counter.push_back(token);
            values.erase(0, pos + delimiter.length());
        }
        counter.push_back(values);

        std::ostringstream oss;
        for (const auto& hw_cnt : counter) {
            oss << hw_cnt << ", ";
        }
        APAJ_INFO("Found hw counter: " << oss.str());

        return counter;
    }

    std::ostringstream getCSVHeader(sampling& sampleStruct, const std::filesystem::path& pathToPapiFldr) {
        std::ostringstream csvHeaderStream;
        auto counterString = readCountersFromPAPICfgFile(sampleStruct, pathToPapiFldr);
        const char* seperator = "";
        for (const auto& it : counterString) {
            csvHeaderStream << seperator << it;
            seperator = ",";
        }

        return csvHeaderStream;
    }

    void papiHandleError(int retVal, std::string msg) {
        APAJ_ERROR(msg);
        printf("PAPI error %d: %s\n", retVal, PAPI_strerror(retVal));
        exit(1);
    }
} // namespace APAJ::PAPI