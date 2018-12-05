#include <ei2f/patterns.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace patterns {

using std::string;

const string new_version = "{NewVersion}";
const string current_version = "{CurrentVersion}";
const string key = "{Key}";

namespace err {

const char * const invalid_config_value = "The following config option is invalid: ";
const char * const mandatory_config_key_not_found = "The following mandatory key could not be found: ";
const char * const backend_type_not_found = "The following Backend.Type is unknown: ";
const char * const transport_type_not_found = "The following Transport.Type is unknown: ";
const char * const updater_type_not_found = "The following Updater.Type is unknown: ";

}

}}}}}
