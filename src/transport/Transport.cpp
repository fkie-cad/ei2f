#include <iostream>
#include <string>

#include <ei2f/exceptions.h>
#include <ei2f/patterns.h>
#include <ei2f/helper.h>
#include <ei2f/transport/Transport.h>

using namespace std;

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace transport {

// INSTANCE MEMBERS

Transport::Transport(const ObjectConfig& config) : Configurable(config)
{
    // INTENTIONALLY LEFT EMPTY
}

Transport::~Transport()
{
    // INTENTIONALLY LEFT EMPTY
}

//STATIC MEMBERS

Transport *
Transport::create_from_uri(const string& uri, const ObjectConfig& config)
{
    string protocol;
    string other;
    left_split(uri, "://", protocol, other);

    try {
        FactoryFunctionPtr create = _factory_map.at(protocol);
        return create(config);
    } catch (out_of_range& e) {
        string msg = patterns::err::transport_type_not_found;
        msg += protocol;
        throw exceptions::InvalidConfiguration(msg);
    }
}

void
Transport::register_factory(const string & transport_type, const Transport::FactoryFunctionPtr factory)
{
    _factory_map[transport_type] = factory;
}

unordered_map<string, Transport::FactoryFunctionPtr> Transport::_factory_map;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::transport
