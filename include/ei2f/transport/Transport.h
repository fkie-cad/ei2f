#ifndef DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_TRANSPORT_H
#define DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_TRANSPORT_H

#include <string>
#include <iostream>
#include <unordered_map>

#include <ei2f/common.h>
#include <ei2f/Configurable.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

/*! \brief Contains the base and framework integrated implementations of a Transport
 *
 * Transports are used when uris need to be converted into resulting istreams
 * This might happen e.g. in Backends that do online lookups or updater.
 */
namespace transport {

using std::string;
using std::istream;

/*! \brief Represents the base class forr a single Transport implementation.
 *
 * This class is the base class for all different types of Transports used
 * to load local or remote (update) resources
 */
class Transport : public Configurable {
    // INSTANCE MEMBERS
    public:
        Transport(const ObjectConfig& config);
        virtual ~Transport();

        Transport& operator=(const Transport&) = delete; //!< Forbidden copying
        Transport(const Transport&) = delete; //!< Forbidden copy construction

        virtual istream& uri_to_istream(const string& uri) = 0;

    // STATIC MEMBERS
    public:
        typedef Transport* (*FactoryFunctionPtr)(const ObjectConfig& config);

        static Transport* create_from_uri(const string& uri, const ObjectConfig& config);
        static void register_factory(const string& protocol, const FactoryFunctionPtr factory);

    private:
        static std::unordered_map<string, FactoryFunctionPtr> _factory_map;
};

typedef std::shared_ptr<Transport> TransportPtr;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::transport

#endif // DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_TRANSPORT_H
