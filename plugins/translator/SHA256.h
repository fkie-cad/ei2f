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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_SHA256_H
#define DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_SHA256_H

#include <string>

#include <ei2f/common.h>
#include <ei2f/ItemInfo.h>
#include <ei2f/translator/Translator.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace translator {

using std::string;

/*! \brief Implements for SHA256 based item info keys
 *
 * This translator implements the following item key conversions
 *
 * Calculated Item      | based on             | Comment
 * ---------------------|----------------------|------------------
 * SHA256.Binary        | SHA256.Hex           | Simple conversion
 * SHA256.Binary        | Item                 | Calculation
 * SHA256.Hex           | SHA256.Binary        | Simple conversion
 * SHA256.Hex           | Item                 | Calculation
 * SHA256.Salted.Binary | SHA256.Salted.Hex    | Simple converison
 * SHA256.Salted.Binary | Item, Salt           | Calculation
 * SHA256.Salted.Hex    | SHA256.Salted.Binary | Simple conversion
 * SHA256.Salted.Hex    | Item, Salt           | Calculation
 */
class SHA256 : public Translator {
    // INSTANCE MEMBERS
    public:
        SHA256();
        virtual ~SHA256();

        virtual void enrich(ItemInfo& item_info, const string& key);
    private:
        void enrich_salted(ItemInfo& item_info, const string& key);
        void enrich_unsalted(ItemInfo& item_info, const string& key);

    // STATIC MEMBERS
    public:
        static Translator * create();

};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::translator

#endif // DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_SHA256_H
