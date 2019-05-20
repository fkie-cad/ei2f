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
