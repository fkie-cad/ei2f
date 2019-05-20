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

#include <stdexcept>
#include <algorithm>

#include <ei2f/exceptions.h>
#include <ei2f/patterns.h>
#include <ei2f/ObjectConfig.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::out_of_range;
using std::invalid_argument;

void
ObjectConfig::extract_unsigned_long(unsigned long& result, const char * const key, bool mandatory, unsigned long default_value, unsigned long min_value, unsigned long max_value) const
{
    string value_str;
    try {
        value_str = this->at(key);
        result = std::stoul(value_str);
        if (result < min_value || result > max_value) {
            throw invalid_argument("");
        }
    } catch (out_of_range& e) {
        if (mandatory) {
            string msg = patterns::err::mandatory_config_key_not_found;
            msg += key;
            throw exceptions::InvalidConfiguration(msg);
        } else {
            result = default_value;
        }
    } catch (invalid_argument& e) {
        string msg = patterns::err::invalid_config_value;
        msg += key;
        msg += "=";
        msg += value_str;
        throw exceptions::InvalidConfiguration(msg);
    }
}

void
ObjectConfig::extract_string(string& result, const char * const key, bool mandatory, const char * const default_value) const
{
    try {
        result = this->at(key);
    } catch (out_of_range& e) {
        if (mandatory) {
            string msg = patterns::err::mandatory_config_key_not_found;
            msg += key;
            throw exceptions::InvalidConfiguration(msg);
        } else {
            result = default_value;
        }
    }
}

void
ObjectConfig::extract_bool(bool& result, const char * const key, bool mandatory, bool default_value) const
{
    string value_str;
    try {
        value_str = this->at(key);
        std::transform(value_str.begin(), value_str.end(), value_str.begin(), ::tolower);
        if (value_str == "true" || (value_str == "1") || (value_str == "yes")) {
            result = true;
        } else if (value_str == "false" || (value_str == "0") || (value_str == "no")) {
            result = false;
        } else {
            throw invalid_argument("");
        }
    } catch (out_of_range& e) {
        if (mandatory) {
            string msg = patterns::err::mandatory_config_key_not_found;
            msg += key;
            throw exceptions::InvalidConfiguration(msg);
        } else {
            result = default_value;
        }
    } catch (invalid_argument& e) {
        string msg = patterns::err::invalid_config_value;
        msg += key;
        msg += "=";
        msg += value_str;
        throw exceptions::InvalidConfiguration(msg);
    }
}

}}}}
