#ifndef APAJ_HPC_DIMENSIONTABLE_HPP
#define APAJ_HPC_DIMENSIONTABLE_HPP

#include <cstdint>
#include <ostream>

namespace APAJ {

    /**
     * @brief This class represents a record in a dimension table consisting of a key and value pair
     */
    class DimensionTable {
    public:
        /**
         * @brief Default constructor
         */
        DimensionTable() = default;

        /**
         * @brief Constructor for a dimension record
         * @param key
         * @param value
         */
        DimensionTable(uint64_t key, uint64_t value) : key(key), value(value) {}

        /**
         * @brief Gets the key for this dimension record
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getKey() const { return key; }

        /**
         * @brief Fets the value for this dimension record
         * @return uint64_t
         */
        [[nodiscard]] uint64_t getValue() const { return value; }

        /**
         * @brief Creates a string representation of this dimension record
         * @param str
         * @param part
         * @return std::ostream
         */
        inline friend std::ostream &operator<<(std::ostream& str, const DimensionTable& part) {
            return str << "DimensionTable: " << std::to_string(part.getKey()) << "," << std::to_string(part.getValue());
        };

    private:
        uint64_t key;
        uint64_t value;
    };

} // namespace APAJ

#endif //APAJ_HPC_DIMENSIONTABLE_HPP
