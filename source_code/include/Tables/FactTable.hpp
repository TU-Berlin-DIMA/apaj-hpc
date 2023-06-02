#ifndef APAJ_HPC_FACTTABLE_HPP
#define APAJ_HPC_FACTTABLE_HPP
#include <Util/APAJLogger.hpp>

#include <cstdint>
#include <ostream>

namespace APAJ {

    /**
     * @brief This class represents a record in the fact table consisting of key, value, and four dimensions keys.
     */
    class FactTable {

    public:
        /**
         * @brief Default constructor
         */
        FactTable() = default;

        /**
         * @brief Constructor for a fact table record
         * @param key
         * @param value
         * @param dimensionTableKey1
         * @param dimensionTableKey2
         * @param dimensionTableKey3
         * @param dimensionTableKey4
         */
        FactTable(const uint64_t& key, const uint64_t& value, const std::vector<uint64_t> dimensionTableKeys)
            : key(key), value(value), dimensionTableKeys(dimensionTableKeys) {}

        /**
         * @brief Gets the key of a fact table record
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getKey() const {
            return key;
        }

        /**
         * @brief Gets the value of a fact table record
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getValue() const {
            return value;
        }

        /**
         * @brief Sets the key of a given dimension with the dimension starting at 1
         * @param newKey
         * @param dimension
         */
        void setDimensionKey(uint64_t newKey, uint64_t dimension) {
            if ((dimension - 1) >= dimensionTableKeys.size()) {
                APAJ_ERROR("Can not set key for dimension " << std::to_string(dimension));
                return;
            }
            dimensionTableKeys[dimension - 1] = newKey;
        }

        /**
         * @brief Gets the key of a given dimension with the dimension starting at 1
         * @param dimension
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getDimensionKey(uint64_t dimension) const {
            if ((dimension - 1) >= dimensionTableKeys.size()) {
                APAJ_ERROR("Can not return key for dimension " << std::to_string(dimension));
                return -1;
            }

            return dimensionTableKeys[dimension - 1];
        }

        /**
         * @brief Creates a string representation of this fact table record and returns this as a ostream
         * @param str
         * @param factTable
         * @return ostream
         */
        inline friend std::ostream& operator<<(std::ostream& str, const FactTable& factTable) {
            return str << "FactTable: " << std::to_string(factTable.getKey()) << ","
                       << std::to_string(factTable.getValue()) << ","
                       << std::to_string(factTable.getDimensionKey(1)) << ","
                       << std::to_string(factTable.getDimensionKey(2)) << ","
                       << std::to_string(factTable.getDimensionKey(3)) << ","
                       << std::to_string(factTable.getDimensionKey(4));
        };
    private:
        uint64_t key;
        uint64_t value;
        std::vector<uint64_t> dimensionTableKeys;
    };

} // namespace APAJ

#endif //APAJ_HPC_FACTTABLE_HPP
