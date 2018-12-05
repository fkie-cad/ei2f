#ifndef DE_FRAUNHOFER_FKIE_EI2F_UPDATER_ITEM_UPDATER_H
#define DE_FRAUNHOFER_FKIE_EI2F_UPDATER_ITEM_UPDATER_H

#include <string>

#include <ei2f/updater/Updater.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

class ObjectConfig;

namespace updater {

using std::string;

/*! \brief Represents a specialized Updater working with individual items
 */
class ItemUpdater : public Updater {
    /********************
     * Instance Members *
     ********************/
    public:
        /*! \brief Initializes the Updater base class object based on the given object config.
         *
         * \param config The configuration to be applied.
         *               The constructor expects the following config keys:
         *               - Updater.ItemKey (optional)
         */
        ItemUpdater(const ObjectConfig& dl);

        /*! \brief Destroy the object
         *
         * This base class destructor does nothing.
         */
        virtual ~ItemUpdater();

        /*! \brief Returns the configured item key to be used for ItemInfos
         *
         * If the implementations uses the item based Backend update API, the
         * implementation needs to know which ItemKey represents the data that is
         * retrieved from the update uri
         */
        const string& item_key(void) const { return _item_key; }

    private:
        string _item_key;
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater

#endif // DE_FRAUNHOFER_FKIE_EI2F_UPDATER_ITEM_UPDATER_H
