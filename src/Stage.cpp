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
#include <assert.h>
#include <fstream>

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/UUIDGenerator.h>

#include <ei2f/Stage.h>
#include <ei2f/updater/Updater.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using namespace std;

Stage::Stage(const ObjectConfig& config): Configurable(config) {
    read_config();
    read_state();
    ensure_directories();

    // Updater
    _updater.reset(Updater::create_from_config(config));

    // Backend
    _backend = create_backend(_backend_uuid);

    // create update event to call update
    // TODO
}

Stage::~Stage() {
    // INTENTIONALLY LEFT EMPTY
}

static std::unordered_map<string, Stage::PersistenceLevel> persistence_level_mapping = {{"Permanent", Stage::PersistenceLevel::Permanent},
                                                                                        {"Temporary", Stage::PersistenceLevel::Temporary},
                                                                                        {"None", Stage::PersistenceLevel::None}};

void Stage::read_config(void) {
    // is temporary stage?
    config().extract_enum(_persistence_level, "Stage.PersistenceLevel", false, PersistenceLevel::Permanent, persistence_level_mapping);
    config().extract_string(_name, "Stage.Name", true, "");
    assert(_name.length() > 0);

    // stage dir (_dir)
    string base_dir;

    if (_persistence_level == None) {
        return;
    } else if (_persistence_level == Temporary) {
        base_dir= "/tmp/spool/domainlistd/";
    } else {
        base_dir= "/var/spool/domainlistd/";
    }

    Poco::Path dir(base_dir);
    dir.pushDirectory(_name);

    try {
        dir.tryParse(config().at("Stage.Dir"));
    } catch (out_of_range& e) {}
    _dir = dir.toString();
    // state file path (_state_path)
    dir.append("state");
    _state_path = dir.toString();

    // backends dir
    dir = Poco::Path(_dir);
    dir.pushDirectory("backends");
    _backends_dir = dir.toString();
}

void Stage::ensure_directories(void)
{
    if (_persistence_level == None) {
        return;
    }

    // stage dir
    Poco::File stage_file(dir());
    stage_file.createDirectories();

    // backends dir
    Poco::File backends_file(backends_dir());
    backends_file.createDirectories();
}

void Stage::read_state(void)
{
    if (_state_path != "") {
        ifstream f(_state_path);
        f >> _backend_version;
        f >> _backend_uuid;
        if (_backend_uuid == "") {
            _backend_version = "0";
        }
    } else {
        _backend_version = "0";
        _backend_uuid = "";
    }
}

void Stage::persist_state(void)
{
    if (_state_path != "") {
        ofstream of(_state_path);
        of << _backend_version << endl;
        of << _backend_uuid << endl;
    }
}

void
Stage::promote_backend(BackendPtr new_backend, const string& new_version)
{
    assert(new_backend.get() != NULL);
    _backend = new_backend;
    _backend_version = new_version;
    _backend_uuid = _backend->uuid();

    persist_state();
}

BackendPtr
Stage::create_backend(const std::string& uuid_hint)
{
    Poco::Path backend_path(backends_dir());
    string uuid = uuid_hint;
    if (uuid_hint == "") {
        Poco::UUIDGenerator gen;
        uuid = gen.createRandom().toString();
    } else {
        uuid = uuid_hint;
    }

    ObjectConfig c = config();
    if (backends_dir() != "") {
        backend_path.append(uuid);
        c["Backend.Path"] = backend_path.toString();
    } else {
        c["Backend.Path"] = "";
    }

    c["Backend.UUID"] = uuid;

    BackendPtr backend(Backend::create_from_config(c));
    return backend;
}

ContainDecision
Stage::contains(ItemInfo& item_info)
{
    return _backend->contains(item_info);
}

void
Stage::try_update(void)
{
    if (_updater) {
        string current_version = backend_version();
        string new_version;
        bool available = _updater->update_available(current_version, new_version);
        if (available) {
            _updater->update_to(*this, new_version);
            // TODO FIXME ERROR HANDLING
        }
    }
}

Stage *
Stage::create_from_config(const ObjectConfig& config)
{
    return new Stage(config);
}

}}}} // Namespace de::fraunhofer::fkie::ei2f
