#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_STD_UNORDERED_SET_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_STD_UNORDERED_SET_H

#include <unordered_set>
#include <string>

#include <ei2f/plugin_common.h>
#include <ei2f/backend/ItemAPI.h>
#include <ei2f/backend/Backend.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using std::string;

/*! \brief Implements a local only memory only Backend
 *
 * This implementation takes the item value defined by "Backend.ItemKey"
 * and uses it for a lookup query against a std::unordered_set datastructure.
 *
 * The following keys are used to configure this Backend in addition
 * to inherited configuration items
 *
 * The following features are supported by the BerkeleyDB backend:
 *  - Memory only storage
 *  - ItemAPI based updaters
 */
class StdUnorderedSet : public Backend, public ItemAPI {
    // TYPE MEMBERS
    private:
        typedef std::unordered_set<string> SetType;

    public:
        class ReadIterator : public ItemAPI::ReadIterator {
        public:
            ReadIterator(StdUnorderedSet& backend);
            virtual ~ReadIterator();
            virtual bool next(ItemInfo& item_info);

        private:
            SetType::const_iterator _pos;
            SetType::const_iterator _end;
            string _item_key;
        };

    // INSTANCE MEMBERS
    public:
        StdUnorderedSet(const ObjectConfig& config);
        virtual ~StdUnorderedSet();
        virtual ContainDecision contains(ItemInfo& item_info);
        virtual void destroy(void);

        // API defintions
        virtual ItemAPI& item_api(void);

        // ItemAPI
        virtual ItemAPI::ReadIterator* create_read_iterator();
        virtual void add(ItemInfo& item_info);
        virtual void remove(ItemInfo& item_info);
        virtual void clear(void);

    private:
        SetType _backend;

    // STATIC MEMBERS
    public:
        static Backend* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_STD_UNORDERED_SET_H
