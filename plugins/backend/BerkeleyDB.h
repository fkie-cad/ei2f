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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BERKELEY_DB_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BERKELEY_DB_H

#include <unordered_set>
#include <string>
#include <db.h>

#include <ei2f/plugin_common.h>
#include <ei2f/PathMixin.h>
#include <ei2f/backend/Backend.h>
#include <ei2f/backend/ItemAPI.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using std::string;

/*! \brief Implements BerkeleyDB as a storage backend
 *
 * The following features are supported by the BerkeleyDB backend:
 *  - Persistent storage
 *  - Memory only storage
 *  - ItemAPI based updaters
 */
class BerkeleyDB : public Backend, public PathMixin, public ItemAPI {
    /********************
     * Nested types     *
     ********************/
    public:
        /*! \brief Implements the required ReadIterator for the ItemAPI
         *
         * The implementation uses a berkeleydb cursor to access the current contents
         */
        class ReadIterator : public ItemAPI::ReadIterator {
            public:
                ReadIterator(BerkeleyDB& db);
                virtual ~ReadIterator();
                virtual bool next(ItemInfo& item_info); //!< Implementation of ReadIterator

            private:
                void close(); //!< Internally closes the database cursor

                DBC* _dbc; //!< Database cursor handle
                string _item_key; //!< the item key to be used to setup an item info
        };

    /********************
     * Instance Members *
     ********************/
    public:
        // Implementation of Backend
        BerkeleyDB(const ObjectConfig& config);
        virtual ~BerkeleyDB();
        virtual ContainDecision contains(ItemInfo& info);
        virtual void destroy(void);
        virtual ItemAPI& item_api(void);

        // Implementation of ItemAPI
        virtual ItemAPI::ReadIterator* create_read_iterator();
        virtual void add(ItemInfo& domain);
        virtual void remove(ItemInfo& domain);
        virtual void clear(void);

    private:
        //! \brief Internalls opens the database
        void open();

        //! \brief Internally closes the database
        void close();

        DB* _db; //!< Handle to berkeley db
        DBTYPE _db_type; //!< Type of DB (HASH, BTREE)
        const char * _name; //!< derived open parameter
        const char * _path; //!< derived open parameter

    /********************
     * Static members   *
     ********************/
    public:
        static Backend* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BERKELEY_DB_H
