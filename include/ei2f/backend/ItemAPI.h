#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_ITEM_API_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_ITEM_API_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <exception>

#include <ei2f/helper.h>
#include <ei2f/common.h>
#include <ei2f/ItemInfo.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using std::string;

/*! \brief Represents the item level API for a storage backend.
 *
 * This class represents the interface for item based backend actions like
 * adding or removing items.
 * it will be used by item level updaters for changing contents of the backend.
 */
class ItemAPI {
    /****************
     * Nested Types *
     ****************/
    public:
        /*! \brief Represents an iterator to read contents of a item oriented backend
         *
         * This iterator does not comply to the std iterators. It is a simple interface
         * to iterate over all of the contents of an item based backend
         */
        class ReadIterator {
            public:
                //! Constructs the iterator
                ReadIterator(void) {}

                //! Destroys the iterator
                virtual ~ReadIterator(void);

                /*! \brief Reads the next item info from the associated backend.
                 *
                 * It is unspecified what happens when underlying backend items are added
                 * or removed.
                 *
                 * \param item_info The ItemInfo that will be filled with the item data
                 * \return Whether the read was successfull, if false is returned there is
                 *         no more item to be read.
                 */
                virtual bool next(ItemInfo& item_info) = 0;
        };

    /********************
     * Instance Members *
     ********************/
    public:
        ItemAPI(void) {}
        virtual ~ItemAPI(void) {}

        virtual ReadIterator* create_read_iterator() = 0;

        /*! \brief Adds an item defined by the ItemInfo parameter.
         *
         * Most backend implementations rely on the ItemInfo retrieval by the
         * key defined by the member function item_key(). Implementations might query
         * other or additional ItemInfo keys when adding the item.
         *
         * \param item_info The ItemInfo object that defines the item to be added to
         *                  the Backend object.
         */
        virtual void add(ItemInfo& item_info) = 0;

        /*! \brief Adds all items defined by a vector of ItemInfo objects to the Backend
         *
         * The implementation in this base class iterates over the entries of the vector
         * and adds each element individually. Implementations might overwrite this
         * functionality with better approaches.
         */
        virtual void add(std::vector<ItemInfo>& item_infos);

        /*! \brief Removes an item defined by the ItemInfo parameter.
         *
         * Most backend implementations rely on the ItemInfo retrieval by the
         * key defined by the member function item_key(). Implementations might query
         * other or additional itemInfo keys when removing the item.
         *
         * \param item_info The ItemInfo object that defines the item to be removed from
         *                  the Backend object.
         */
        virtual void remove(ItemInfo& item_info) = 0;

        /*! \brief Removes all items defined by a vector of ItemInfo objects of the Backend
         *
         * The implementation in this base class iterates over the entries of the vector
         * and removes each element individually. Implementations might overwrite this
         * functionality with better approaches.
         */
        virtual void remove(std::vector<ItemInfo>& item_infos);

        /*! \brief Removes all list entries from the backend.
         *
         * The implementation of this virtual method in the current base class
         * will throw an exception to indicate that it is not implemented.
         * If not implemented, updaters that use this method will fail updating
         * the backend. After calling this method, the backend can be used the
         * same way as before (compare to destroy method).
         */
        virtual void clear(void) = 0;
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_ITEM_API_H
