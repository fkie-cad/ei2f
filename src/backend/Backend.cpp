#include <iostream>

#include <ei2f/exceptions.h>
#include <ei2f/patterns.h>
#include <ei2f/backend/Backend.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;

// class Backend
Backend::Backend(const ObjectConfig& config) : Configurable(config)
{
    // Mandatory Parameters
    config.extract_string(_uuid, "Backend.UUID", true, "");

    // Optional Parameters
    config.extract_string(_item_key, "Backend.ItemKey", false, "Item");
}

Backend::~Backend()
{
    // INTENTIONALLY LEFT EMPTY
}

ItemAPI&
Backend::item_api(void)
{
    throw exceptions::APINotSupported("The item API is not supported by this backend implementation. This might indicate a misconfiguration.");
}

BinaryAPI&
Backend::binary_api(void)
{
    throw exceptions::APINotSupported("The binary API is not supported by this backend implementation. This might indicate a misconfiguration.");
}

void
Backend::flush(void)
{
    // INTENTIONALLY LEFT EMPTY
}

void
Backend::destroy(void)
{
    // INTENTIONALLY LEFT EMPTY
}

Backend *
Backend::create_from_config(const ObjectConfig& config)
{
    string backend_type;
    config.extract_string(backend_type, "Backend.Type", true, "");

    FactoryFunctionPtr create;
    try {
        create = _factory_map.at(backend_type);
    } catch (out_of_range& e) {
        string msg = patterns::err::backend_type_not_found;
        msg += backend_type;
        throw exceptions::InvalidConfiguration(msg);
    }
    return create(config);
}

void
Backend::register_factory(const string& backend_type, const Backend::FactoryFunctionPtr factory)
{
    _factory_map[backend_type] = factory;
}

unordered_map<string, Backend::FactoryFunctionPtr> Backend::_factory_map;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend
