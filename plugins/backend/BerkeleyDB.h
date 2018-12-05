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
