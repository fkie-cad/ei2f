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
