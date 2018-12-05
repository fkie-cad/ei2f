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
