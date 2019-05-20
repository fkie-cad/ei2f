/*
    Copyright (c) 2018-2019 Fraunhofer FKIE

    This file is part of EI2F.

    EI2F is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    EI2F is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with EI2F.  If not, see <http://www.gnu.org/licenses/>.
*/

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
