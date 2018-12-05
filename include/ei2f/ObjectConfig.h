#ifndef DE_FRAUNHOFER_FKIE_EI2F_OBJECT_CONFIG_H
#define DE_FRAUNHOFER_FKIE_EI2F_OBJECT_CONFIG_H

#include <string>
#include <unordered_map>
#include <stdexcept>

#include <ei2f/patterns.h>
#include <ei2f/exceptions.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::string;

/*! \brief Represents the configuration for a Configurable object.
 *
 * Objects of this type are used to configure objects. Currently this
 * implementation stores all keys and values as string.
 * This class additionally provides methods to parse some specific data types.
 */
class ObjectConfig : public std::unordered_map<string,string> {
    /********************
     * Instance Members *
     ********************/
    public:
        /*! \brief Extracts a single unsigned long from the config
         *
         * If the parameter is not mandatory and the key cannot be found
         * the default value will be written to the result.
         *
         * An InvalidConfiguration exception will be thrown when:
         *   - the value string cannot be parsed
         *   - the value does not fullfill min_value or max_value
         *   - a mandatory key cannot be found
         *
         * \param[out] result The result of the extraction will be saved in the referenced variable.
         * \param      key    The key that should be searched for in the config.
         * \param      mandatory Defines whether the key must exist in the config.
         * \param      default_value The default value to use in case of an optional parameter.
         * \param      min_value The minimum value that should be accepted.
         * \param      max_value The maximumg value that should be accepted.
         */
        void extract_unsigned_long(unsigned long& result,
                                   const char * const key,
                                   bool mandatory,
                                   unsigned long default_value,
                                   unsigned long min_value,
                                   unsigned long max_value) const;

        /*! \brief Extracts a single string from the config
         *
         * If the parameter is not mandatory and the key cannot be found
         * the default value will be written to the result.
         *
         * An InvalidConfiguration exception will be thrown when:
         *   - a mandatory key cannot be found
         *
         * \param[out] result The result of the extraction will be saved in the referenced variable.
         * \param      key    The key that should be searched for in the config.
         * \param      mandatory Defines whether the key must exist in the config.
         * \param      default_value The default value to use in case of an optional parameter.
         */
        void extract_string(std::string& result,
                            const char * const key,
                            bool mandatory,
                            const char * const default_value) const;

        /*! \brief Extracts a single bool from the config
         *
         * If the parameter is not mandatory and the key cannot be found
         * the default value will be written to the result.
         *
         * An InvalidConfiguration exception will be thrown when:
         *   - the value string cannot be parsed
         *   - a mandatory key cannot be found
         *
         * \param[out] result The result of the extraction will be saved in the referenced variable.
         * \param      key    The key that should be searched for in the config.
         * \param      mandatory Defines whether the key must exist in the config.
         * \param      default_value The default value to use in case of an optional parameter.
         */
        void extract_bool(bool& result,
                          const char * const key,
                          bool mandatory,
                          bool default_value) const;

        /*! \brief Extracts a enum from the config
         *
         * If the parameter is not mandatory and the key cannot be found
         * the default value will be written to the result.
         *
         * An InvalidConfiguration exception will be thrown when:
         *   - the value string cannot be parsed
         *   - a mandatory key cannot be found
         *   - no mapping can be found for the value string
         *
         * \tparam     ENUM_T The type of the Enum to be used
         * \param[out] result The result of the extraction will be saved in the referenced variable.
         * \param      key    The key that should be searched for in the config.
         * \param      mandatory Defines whether the key must exist in the config.
         * \param      default_value The default value to use in case of an optional parameter.
         * \param      mapping The mapping between value strings and ENUM_T values.
         */
        template <typename ENUM_T>
        void extract_enum(ENUM_T& result, const char * const key, bool mandatory, ENUM_T default_value, const std::unordered_map<string, ENUM_T>& mapping) const;
};

template <typename ENUM_T>
void
ObjectConfig::extract_enum(ENUM_T& result, const char * const key, bool mandatory, ENUM_T default_value, const std::unordered_map<string, ENUM_T>& mapping) const
{
    string value_str;
    try {
        value_str = this->at(key);
    } catch (std::out_of_range& e) {
        if (mandatory) {
            string msg = patterns::err::mandatory_config_key_not_found;
            msg += key;
            throw exceptions::InvalidConfiguration(msg);
        } else {
            result = default_value;
            return;
        }
    }
    try {
        result = mapping.at(value_str);
        return;
    } catch (std::out_of_range& e) {
        string msg = patterns::err::invalid_config_value;
        msg += key;
        msg += "=";
        msg += value_str;
        throw exceptions::InvalidConfiguration(msg);
    }
}

}}}}

#endif // DE_FRAUNHOFER_FKIE_EI2F_OBJECT_CONFIG_H
