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

#include <ei2f/ObjectConfig.h>
#include <ei2f/updater/ItemUpdater.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using namespace std;

ItemUpdater::ItemUpdater(const ObjectConfig& config) : Updater(config)
{
    // Needed parameters
    // None

    // Optional parameters
    config.extract_string(_item_key, "Updater.ItemKey", false, "Item");
}

ItemUpdater::~ItemUpdater()
{

}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater
