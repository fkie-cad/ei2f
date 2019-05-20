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

#ifndef DE_FRAUNHFOER_FKIE_EI2F_STAGE_H
#define DE_FRAUNHFOER_FKIE_EI2F_STAGE_H

#include <map>
#include <list>
#include <memory>
#include <string>

#include <ei2f/common.h>
#include <ei2f/Configurable.h>
#include <ei2f/transport/Transport.h>
#include <ei2f/updater/Updater.h>
#include <ei2f/backend/Backend.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using updater::Updater;
using backend::Backend;
using backend::BackendPtr;
using updater::Updater;
using updater::UpdaterPtr;
using std::string;

class Stage : public Configurable {
    /****************
     * Nested Types *
     ****************/
    public:
        enum PersistenceLevel { Permanent, Temporary, None };


    /********************
     * Instance Members *
     ********************/
    public:
        Stage(const ObjectConfig & config);
        ~Stage();
        Stage& operator=(const Stage&) = delete;
        Stage(const Stage&) = delete;

        const string& dir(void) const { return _dir; }
        const string& backends_dir(void) const { return _backends_dir; }
        const string& name(void) const { return _name; }

        BackendPtr backend(void) { return _backend; }
//        TransportPtr transport(void) { return _transport; }
        UpdaterPtr updater(void) { return _updater; }

        void promote_backend(BackendPtr new_backend, const string& new_version);
        BackendPtr create_backend(void) { return create_backend(""); }

        void try_update(void);
        const string& backend_version(void) const { return _backend_version; }

        ContainDecision contains(ItemInfo& item_info);

    protected:
        void read_config(void);
        void read_state(void);
        void persist_state(void);
        void ensure_directories(void);
        BackendPtr create_backend(const string& name);

//        TransportPtr _transport;
        BackendPtr _backend;
        UpdaterPtr _updater;

        string _dir;
        string _backends_dir;
        string _name;
        PersistenceLevel _persistence_level;

        // STATE
        string _state_path;
        string _backend_version;
        string _backend_uuid;

    /******************
     * Static Members *
     ******************/
    public:
        static Stage* create_from_config(const ObjectConfig& config);
};

typedef std::shared_ptr<Stage> StagePtr;

}}}} // Namespace de::fraunhofer::fkie::EI2F

#endif // DE_FRAUNHFOER_FKIE_EI2F_STAGE_H

