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
