#ifndef DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_HTTP_H
#define DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_HTTP_H

#include <fstream>
#include <memory>

// TODO TRY MOVING TO CPP FILE
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>

#include <ei2f/plugin_common.h>
#include <ei2f/transport/Transport.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace transport {

using std::string;

class HTTP : public Transport {
    public:
        HTTP(const ObjectConfig& config);
        virtual ~HTTP();

        virtual std::istream& uri_to_istream(const string& raw_uri);

    private:
        std::shared_ptr<Poco::Net::HTTPClientSession> _session;
        std::shared_ptr<Poco::Net::HTTPResponse> _res;

    // STATIC MEMBERS
    public:
        static Transport * create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::transport

#endif // DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_HTTP_H
