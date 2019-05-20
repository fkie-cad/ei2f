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

#include <Poco/StringTokenizer.h>

#include <ei2f/common.h>
#include <ei2f/Stage.h>
#include <ei2f/exceptions.h>
#include <ei2f/transport/Transport.h>
#include <ei2f/backend/BinaryAPI.h>
#include <ei2f/backend/Backend.h>

#include "Binary.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using namespace std;
using namespace Poco;
using de::fraunhofer::fkie::ei2f::backend::Backend;
using de::fraunhofer::fkie::ei2f::transport::Transport;
using de::fraunhofer::fkie::ei2f::transport::TransportPtr;

Binary::Binary(const ObjectConfig& config) : Updater(config)
{
    // Needed configs
    // None

    // Optional configs
    // None
}

Binary::~Binary()
{

}

/*
bool
Binary::update_available(const string& current_version, string& new_version)
{
    string descriptor_uri = update_descriptor_uri_pattern();
    if (descriptor_uri == "") {
        // Base implementation always says that update is available and sets new version to current version
        new_version = current_version;
        return true;
    } else {
        replace_all(descriptor_uri, patterns::version_pattern, current_version);
        TransportPtr transport(Transport::create_from_url(descriptor_uri, config()));
        istream & input = transport->uri_to_istream(descriptor_uri);
        input.getline(input, new_version);
        return new_version != current_version;
    }
}
*/

void
Binary::stream_to_blob(istream& input, BinaryAPI::BLOB& blob)
{
    input.read(blob.data, blob.size);

    if (!input.good()) {
        throw exceptions::UpdateFailed("The size of the received binary update data was too small.");
    }

    char dummy;
    input.read(&dummy, 1);

    if (input.good()) {
        throw exceptions::UpdateFailed("The size of the received binary update data was too large.");
    }
}

void
Binary::update_to(Stage& stage, const string& new_version)
{
    BackendPtr old_backend;
    BackendPtr new_backend;

    try {
        old_backend = stage.backend();
        if (in_place()) {
            new_backend = stage.backend();
        } else {
            new_backend = stage.create_backend();
        }

        BinaryAPI& new_backend_api = new_backend->binary_api();

        string update_uri = uri_pattern();
        pattern_to_uri(update_uri, stage, new_version);

        TransportPtr transport(Transport::create_from_uri(update_uri, stage.config()));
        istream & input = transport->uri_to_istream(update_uri);

        BinaryAPI::BLOB new_blob;
        new_backend_api.binary_ref(new_blob);
        stream_to_blob(input, new_blob);

        new_backend->flush();

    } catch (...) {
        if (!in_place() && new_backend) {
            new_backend->destroy();
            new_backend.reset();
        }
        throw;
    }

    stage.promote_backend(new_backend, new_version);

    // Promote new store in case of non in place update
    if (!in_place()) {
        // Cleanup and exchange
        old_backend->destroy();
    }
}

Updater*
Binary::create_from_config(const ObjectConfig& config)
{
    return new Binary(config);
}

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::updater::Binary: Initializing Plugin" << endl;
    Updater::register_factory("Binary", Binary::create_from_config);
    Updater::register_factory("de::fraunhofer::fkie::ei2f::updater::Binary", Binary::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater
