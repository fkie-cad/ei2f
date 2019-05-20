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
