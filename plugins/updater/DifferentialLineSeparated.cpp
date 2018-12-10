#include <iostream>
#include <string>

#include <ei2f/Stage.h>
#include <ei2f/ItemInfo.h>
#include <ei2f/ObjectConfig.h>
#include <ei2f/transport/Transport.h>
#include <ei2f/backend/ItemAPI.h>
#include <ei2f/backend/Backend.h>

#include "DifferentialLineSeparated.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using transport::Transport;
using transport::TransportPtr;
using namespace std;

DifferentialLineSeparated::DifferentialLineSeparated(const ObjectConfig& config) : Line(config)
{
    // Needed parameters
    config.extract_string(_add_uri_pattern, "Updater.DiffAddUriPattern", true, "");
    config.extract_string(_remove_uri_pattern, "Updater.DiffRemoveUriPattern", true, "");
    config.extract_string(_version_uri_pattern, "Updater.VersionUriPattern", true, "");

    // Optional Parameters
    // None
}

DifferentialLineSeparated::~DifferentialLineSeparated()
{

}

void
DifferentialLineSeparated::remove_lines_from_backend(istream& input, ItemAPI& item_api)
{
    // Line Buffer
    string line;

    ItemInfo item_info;
    while (getline(input, line)) {
        item_info.reset(item_key(), line);
        item_api.remove(item_info);
    }
}

void
DifferentialLineSeparated::inplace_update_to(Stage& stage, const string& new_version)
{
    // Access to the promoted backend
    BackendPtr backend = stage.backend();
    ItemAPI& backend_api = backend->item_api();

    // get add-uri
    string add_uri = add_uri_pattern();
    pattern_to_uri(add_uri, stage, new_version);

    // create transport to by add-differential uri
    TransportPtr transport(Transport::create_from_uri(add_uri, stage.config()));

    // download add-diff and update
    istream& add_input = transport->uri_to_istream(add_uri);
    add_lines_to_backend(add_input, backend_api);

    // get remove-uri
    string remove_uri = remove_uri_pattern();
    pattern_to_uri(remove_uri, stage, new_version);

    // download remove-diff and update
    istream& remove_input = transport->uri_to_istream(remove_uri);
    remove_lines_from_backend(remove_input, backend_api);

    // promote the current backend as new version
    stage.promote_backend(backend, new_version);
}

void
DifferentialLineSeparated::noninplace_update_to(Stage& stage, const string& new_version)
{
    BackendPtr new_backend;
    BackendPtr del_backend;
    BackendPtr old_backend;

    try {
        old_backend = stage.backend();
        new_backend = stage.create_backend();
        del_backend = stage.create_backend();
        ItemAPI& new_backend_api = new_backend->item_api();
        ItemAPI& del_backend_api = del_backend->item_api();
        ItemAPI& old_backend_api = old_backend->item_api();

        string add_uri = add_uri_pattern();
        pattern_to_uri(add_uri, stage, new_version);

        TransportPtr transport(Transport::create_from_uri(add_uri, stage.config()));

        istream& add_input = transport->uri_to_istream(add_uri);
        add_lines_to_backend(add_input, new_backend_api);

        string remove_uri = remove_uri_pattern();
        pattern_to_uri(remove_uri, stage, new_version);

        istream& remove_input = transport->uri_to_istream(remove_uri);
        add_lines_to_backend(remove_input, del_backend_api);

        // iterate over remaining items in old backend,
        // check against del_backend and add it to new_backend
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

    } catch (std::exception& e) {
        if (new_backend) new_backend->destroy();
        if (del_backend) del_backend->destroy();
        throw;
    }

    stage.promote_backend(new_backend, new_version);
    old_backend->destroy();
}

void
DifferentialLineSeparated::update_to(Stage& stage, const string& new_version)
{
    try {
        if (!in_place()) {
            noninplace_update_to(stage, new_version);
        } else {
            inplace_update_to(stage, new_version);
        }
    } catch (std::exception& e) {
        Line::update_to(stage, new_version);
    }
}

Updater*
DifferentialLineSeparated::create_from_config(const ObjectConfig& config)
{
    return new DifferentialLineSeparated(config);
}

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::updater::DifferentialLineSeparated: Initializing Plugin" << endl;
    Updater::register_factory("DifferentialLineSeparated", DifferentialLineSeparated::create_from_config);
    Updater::register_factory("de::fraunhofer::fkie::ei2f::updater::DifferentialLineSeparated", DifferentialLineSeparated::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater
