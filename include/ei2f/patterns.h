#ifndef DE_FRAUNHOFER_FKIE_EI2F_PATTERNS_H
#define DE_FRAUNHOFER_FKIE_EI2F_PATTERNS_H

#include <string>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace patterns {

using std::string;

extern const string new_version;
extern const string current_version;
extern const string key;

namespace err {
    extern const char * const mandatory_config_key_not_found;
    extern const char * const invalid_config_value;
    extern const char * const backend_type_not_found;
    extern const char * const transport_type_not_found;
    extern const char * const updater_type_not_found;
}

}}}}}


#endif // DE_FRAUNHOFER_FKIE_EI2F_PATTERNS_H
