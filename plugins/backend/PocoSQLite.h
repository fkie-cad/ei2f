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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_POCOSQLITE_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_POCOSQLITE_H

#include <ei2f/plugin_common.h>

#include <unordered_set>
#include <string>

#include <Poco/Data/Session.h>
#include <Poco/Data/RecordSet.h>

#include <ei2f/PathMixin.h>
#include <ei2f/backend/Backend.h>
#include <ei2f/backend/ItemAPI.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using std::string;
using Poco::SharedPtr;
using Poco::Data::RecordSet;

/*! \brief Implements sqlite database as a storage backend
 *
 * This implementation uses the PocoData library to access
 * a sqlite database.
 *
 * The following features are supported by the BerkeleyDB backend:
 *  - Persistent storage
 *  - Memory only storage
 *  - ItemAPI based updaters
 */
class PocoSQLite : public Backend, public PathMixin, public ItemAPI {

        /*! \brief Implements the required ReadIterator for the ItemAPI
         *
         * The implementation uses Poco's result set's db cursor functionality.
         */
        class ReadIterator : public ItemAPI::ReadIterator {
        public:
            ReadIterator(PocoSQLite& backend);
            virtual ~ReadIterator();
            virtual bool next(ItemInfo& item_info);

        private:
            RecordSet _record_set;
            RecordSet::Iterator _pos;
            RecordSet::Iterator _end;
            string _item_key;
        };

    /********************
     * Instance Members *
     ********************/
    public:
        PocoSQLite(const ObjectConfig& config);
        virtual ~PocoSQLite();
        virtual ContainDecision contains(ItemInfo& item_info);
        virtual void destroy();

        // ItemAPI
        virtual ItemAPI::ReadIterator* create_read_iterator();
        virtual void add(ItemInfo& item_info);
        virtual void remove(ItemInfo& item_info);
        virtual void clear(void);
        virtual ItemAPI& item_api();


    /********************
     * Static members *
     ********************/
    private:
        SharedPtr<Poco::Data::Session> _session;
        SharedPtr<Poco::Data::Statement> _add_statement;
        SharedPtr<Poco::Data::Statement> _remove_statement;
        SharedPtr<Poco::Data::Statement> _contains_statement;

        // Query buffers
        string _item; //!< Item variable connected to _*_statements
        int _count; //!< Counter varriable connected to _contains_statement

    // STATIC MEMBERS
    public:
        static Backend* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_POCOSQLITE_H
