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
