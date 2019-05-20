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

#include <ei2f/Stage.h>
#include <ei2f/ItemInfo.h>
#include <ei2f/transport/Transport.h>
#include <ei2f/backend/ItemAPI.h>
#include <ei2f/backend/Backend.h>

#include "DifferentialLine.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using namespace std;
using namespace Poco;
using backend::Backend;
using transport::Transport;
using transport::TransportPtr;

DifferentialLine::DifferentialLine(const ObjectConfig& config) : Line(config)
{
    // Needed configs
    // None

    // Optional configs
    // None
}

DifferentialLine::~DifferentialLine()
{

}

void
DifferentialLine::key_value_event(const string& key, const string& value)
{
    // TODO FIXME IMPLEMENT
}

void
DifferentialLine::new_line_event(const std::string & line, ItemAPI& new_backend_api, ItemAPI& del_backend_api)
{
    StringTokenizer tokenizer(line, ",", StringTokenizer::TOK_TRIM);

    if (tokenizer.count() == 0) {
        // ignore empty lines
    } else if (tokenizer.count() < 2) {

        // TODO Error handling, e.g. Through Exception

    } else if (tokenizer[0].compare("-") == 0) {
        ItemInfo item_info(item_key(), tokenizer[1]);
        if (in_place()) {
            del_backend_api.remove(item_info);
        } else {
            del_backend_api.add(item_info);
        }

    } else if (tokenizer[0].compare("+") == 0) {
        ItemInfo item_info(item_key(), tokenizer[1]);
        new_backend_api.add(item_info);

    } else if (tokenizer.count() < 3) {

        // TODO Error handling, e.g. Through Exception

    } else if (tokenizer[0].compare("=") == 0) {

        key_value_event(tokenizer[1], tokenizer[2]);

    } else {

        // TODO Error handling, e.g. Through Exception

    }
}

void
DifferentialLine::inplace_update_to(Stage& stage, const string& new_version)
{
    BackendPtr backend = stage.backend();
    ItemAPI& backend_api = backend->item_api();

    string update_uri = uri_pattern();
    pattern_to_uri(update_uri, stage, new_version);

    TransportPtr transport(Transport::create_from_uri(update_uri, stage.config()));
    istream & input = transport->uri_to_istream(update_uri);

    string line;
    while (getline(input, line)) {
        new_line_event(line, backend_api, backend_api);
    }

    backend->flush();
    stage.promote_backend(backend, new_version);
}


void
DifferentialLine::noninplace_update_to(Stage& stage, const string& new_version)
{
    BackendPtr old_backend;
    BackendPtr new_backend;
    BackendPtr del_backend;

    try {
        old_backend = stage.backend();
        new_backend = stage.create_backend();
        del_backend = stage.create_backend();

        ItemAPI& old_backend_api = old_backend->item_api();
        ItemAPI& new_backend_api = new_backend->item_api();
        ItemAPI& del_backend_api = del_backend->item_api();

        string update_uri = uri_pattern();
        pattern_to_uri(update_uri, stage, new_version);

        TransportPtr transport(Transport::create_from_uri(update_uri, stage.config()));
        istream & input = transport->uri_to_istream(update_uri);

        string line;
        while (getline(input, line)) {
            new_line_event(line, new_backend_api, del_backend_api);
        }

        // iterate over remaining items in old backend,
        // check against remove_backend and add it to target_backend
        std::shared_ptr<ItemAPI::ReadIterator> iter(old_backend_api.create_read_iterator());

        ItemInfo item_info;
        while (iter->next(item_info)) {
            if (del_backend->contains(item_info) != ContainDecision::contained) {
                new_backend_api.add(item_info);
            }
        }

        del_backend->destroy();
        del_backend.reset();

        new_backend->flush();

    } catch (...) {
        if (del_backend) del_backend->destroy();
        if (new_backend) new_backend->destroy();
        // old_backend must continue to exist in this case, so NO DESTROY
        throw;
    }

    stage.promote_backend(new_backend, new_version);
    old_backend->destroy();
}

void
DifferentialLine::update_to(Stage& stage, const string& new_version)
{
    try {
        if(in_place()) {
            inplace_update_to(stage, new_version);
        } else {
            noninplace_update_to(stage, new_version);
        }
    } catch (...) {
        Line::update_to(stage, new_version);
    }
}

Updater*
DifferentialLine::create_from_config(const ObjectConfig& config)
{
    return new DifferentialLine(config);
}

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::updater::DifferentialLine: Initializing Plugin" << endl;
    Updater::register_factory("DifferentialLine", DifferentialLine::create_from_config);
    Updater::register_factory("de::fraunhofer::fkie::ei2f::updater::DifferentialLine", DifferentialLine::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater
