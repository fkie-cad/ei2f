#include <iostream>
#include <string>

#include <Poco/StringTokenizer.h>

#include <ei2f/Stage.h>
#include <ei2f/transport/Transport.h>
#include <ei2f/backend/ItemAPI.h>
#include <ei2f/backend/Backend.h>

#include "Line.h"

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

Line::Line(const ObjectConfig& config) : ItemUpdater(config)
{
    // Needed configs
    // None

    // Optional configs
    // None
}

Line::~Line()
{

}

void
Line::add_lines_to_backend(istream& input, ItemAPI& item_api)
{
    // Line Buffer
    string line;

    ItemInfo item_info;
    while (getline(input, line)) {
        item_info.reset(item_key(), line);
        item_api.add(item_info);
    }
}

void
Line::update_to(Stage& stage, const string& new_version)
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
        ItemAPI& new_backend_api = new_backend->item_api();

        string update_uri = uri_pattern();
        pattern_to_uri(update_uri, stage, new_version);

        TransportPtr transport(Transport::create_from_uri(update_uri, stage.config()));
        istream & input = transport->uri_to_istream(update_uri);

        new_backend_api.clear();
        add_lines_to_backend(input, new_backend_api);

        new_backend->flush();

    } catch (...) {
        if (!in_place() && new_backend) {
            new_backend->destroy();
        }
        throw;
    }

    stage.promote_backend(new_backend, new_version);
    if (!in_place()) {
        old_backend->destroy();
    }
}

Updater*
Line::create_from_config(const ObjectConfig& config)
{
    return new Line(config);
}

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::updater::Line: Initializing Plugin" << endl;
    Updater::register_factory("Line", Line::create_from_config);
    Updater::register_factory("de::fraunhofer::fkie::ei2f::updater::Line", Line::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater
