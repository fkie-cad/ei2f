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

#include <ei2f/Stage.h>
#include <ei2f/patterns.h>
#include <ei2f/transport/Transport.h>
#include <ei2f/updater/Updater.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using transport::Transport;
using transport::TransportPtr;
using namespace std;

Updater::Updater(const ObjectConfig& config) : Configurable(config)
{
    // Needed parameters
    config.extract_string(_uri_pattern, "Updater.UriPattern", true, "");

    // Optional parameters
    config.extract_bool(_in_place, "Updater.InPlace", false, false);
    config.extract_string(_version_uri_pattern, "Updater.VersionUriPattern", false, "");
}

Updater::~Updater()
{

}

bool
Updater::update_available(const string& current_version, string& new_version)
{
    string version_uri = version_uri_pattern();
    if (version_uri == "") {
        // Base implementation always says that update is available and sets new version to current version
        new_version = current_version;
        return true;
    } else {
        replace_all(version_uri, patterns::current_version, current_version);
        TransportPtr transport(Transport::create_from_uri(version_uri, config()));
        istream & input = transport->uri_to_istream(version_uri);
        new_version = "";
        getline(input, new_version);
        if (new_version == "") {
            new_version = current_version;
            return true;
        } else {
            return new_version != current_version;
        }
    }
}

void
Updater::pattern_to_uri(string& uri, Stage& stage, const string& new_version)
{
    replace_all(uri, patterns::new_version, new_version);
}

Updater*
Updater::create_from_config(const ObjectConfig& config)
{
    string updater_type;
    config.extract_string(updater_type, "Updater.Type", false, "None");

    if (updater_type == "None") {
        return NULL;
    }
    FactoryFunctionPtr create;
    try {
        create = _factory_map.at(updater_type);
    } catch (out_of_range& e) {
        string msg = patterns::err::updater_type_not_found;
        msg += updater_type;
        throw exceptions::InvalidConfiguration(msg);
    }

    return create(config);
}

void
Updater::register_factory(const string& updater_type, const Updater::FactoryFunctionPtr factory)
{
    _factory_map[updater_type] = factory;
}

unordered_map<string, Updater::FactoryFunctionPtr> Updater::_factory_map;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater
