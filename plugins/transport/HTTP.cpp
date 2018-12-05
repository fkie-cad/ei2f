#include <string>

#include <Poco/Net/HTTPSessionInstantiator.h>
#include <Poco/Net/HTTPSSessionInstantiator.h>
#include <Poco/Net/HTTPSessionFactory.h>
#include <Poco/Net/HTTPClientSession.h>

#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/RejectCertificateHandler.h>
#include <Poco/Net/SSLManager.h>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <Poco/URI.h>
#include <Poco/Exception.h>

#include <iostream>

#include "HTTP.h"

#define MAX_REDIRECT 16

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace transport {

using namespace Poco::Net;
using namespace Poco;
using namespace std;

HTTP::HTTP(const ObjectConfig& config) : Transport(config) {

}

HTTP::~HTTP() {

}

istream&
HTTP::uri_to_istream(const string & raw_uri) {
    URI initial_uri(raw_uri);
    URI uri(raw_uri);

    // HTTP REDIRECTION LOOP
    for (int i = 0; i < MAX_REDIRECT; ++i) {
        // prepare input
        if (uri.getScheme().compare("") == 0) uri.setScheme(initial_uri.getScheme());
        if (uri.getHost().compare("") == 0) uri.setHost(initial_uri.getHost());
        if (uri.getPort() == 0) uri.setPort(initial_uri.getPort());

        // setup object state
        if (!_session) {
            _session.reset(HTTPSessionFactory::defaultFactory().createClientSession(uri));
        } else if (uri.getScheme().compare(initial_uri.getScheme()) != 0
                    || uri.getUserInfo().compare(initial_uri.getUserInfo()) != 0
                    || uri.getHost().compare(initial_uri.getHost()) != 0
                    || uri.getPort() == initial_uri.getPort()) {
            _session.reset(HTTPSessionFactory::defaultFactory().createClientSession(uri));
        }
        _res.reset(new HTTPResponse());

        // prepare for request
        string req_str(uri.getPathAndQuery());
        if (req_str.empty()) req_str = "/";

        // send request
        HTTPRequest req(HTTPRequest::HTTP_GET, req_str, HTTPMessage::HTTP_1_1);
        _session->sendRequest(req);

        // get response
        istream &is = _session->receiveResponse(*_res);
        int status = _res->getStatus();

        switch (status) {
            case 301:
            case 302:
            case 303:
            case 307:
                uri = URI(_res->get("Location"));

                // Read body junk til EOF
                is.ignore(numeric_limits<streamsize>::max(), EOF);
                continue;
            case 200:
                break;
            default:
                // TODO FIXME Error handling, e.g. throw UpdateNotFound
                break;
        }

        return is;
    }

    // TODO Error handling, e.g. throw UpdateNotFound
}

Transport *
HTTP::create_from_config(const ObjectConfig& config) {
    return new HTTP(config);
}

static void
configure_session_factory(void);

void
configure_session_factory(void) {
    HTTPSessionFactory::defaultFactory().registerProtocol("http", new HTTPSessionInstantiator);
    HTTPSessionFactory::defaultFactory().registerProtocol("https", new HTTPSSessionInstantiator);
//    HTTPSessionFactory::defaultFactory().setProxy("www-cache.fkie.fraunhofer.de", 3128);

    // TODO FIX FOR Certificate checks
    SharedPtr<InvalidCertificateHandler> ptrCert = new AcceptCertificateHandler(false);
//    SharedPtr<InvalidCertificateHandler> ptrCert = new RejectCertificateHandler(false);
    const Poco::Net::Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_STRICT, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    SSLManager::instance().initializeClient(0, ptrCert, context);
}


static void on_library_load(void) __attribute__((constructor));

void
on_library_load(void)
{
    cout << "de::fraunhofer::fkie::ei2f::transport::HTTP: Initializing Plugin" << endl;
    configure_session_factory();
    Transport::register_factory("http", HTTP::create_from_config);
    Transport::register_factory("https", HTTP::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::transport
