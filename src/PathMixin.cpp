#include <ei2f/PathMixin.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

PathMixin::PathMixin(const ObjectConfig& config, const char * config_key)
{
    config.extract_string(_path, config_key, true, "");
}

PathMixin::~PathMixin(void)
{
}

}}}} // Namespace de::fraunhofer::fkie::ei2f
