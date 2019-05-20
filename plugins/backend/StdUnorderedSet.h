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
