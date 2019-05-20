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

#include <string>
#include <exception>

#include <ei2f/ItemInfo.h>
#include <ei2f/translator/Translator.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::out_of_range;
using translator::Translator;
using translator::TranslatorPtr;

const char * const cyclic_error_message = "A recursive call in ItemInfo Keys was detected. Usually this means that translators have a circular dependency.";

string& ItemInfo::get(const string& key)
{
    if (_lookup.count(key) > 0) {
        return _lookup.at(key);
    }

    // This will throw an exception if the key was already queried in get before in this
    // recursive call
    CycleGuard protector(_seen_db, key, cyclic_error_message);

    TranslatorPtr translator(Translator::create_from_key(key));

    if (!translator) {
        throw TranslatorNotFound();
    }
    translator->enrich(*this, key);
    return _lookup.at(key);
}

void
ItemInfo::set(const string& key, const string& value)
{
    _lookup[key] = value;
}

void
ItemInfo::reset(void)
{
    _lookup.clear();
}

}}}} // Namespace de::fraunhofer::fkie::ei2f
