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

#include <iostream>

#include <ei2f/common.h>
#include <ei2f/exceptions.h>
#include <ei2f/translator/Translator.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace translator {

using namespace std;

Translator::Translator()
{
    // INTENTIONALLY LEFT EMPTY
}

Translator::~Translator()
{
    // INTENTIONALLY LEFT EMPTY
}

Translator *
Translator::create_from_key(const string& key)
{
    FactoryFunctionPtr create;
    try {
        create = _factory_map.at(key);
    } catch (out_of_range& e) {
        string msg = "Unknown key '";
        msg += key;
        msg += "' for ItemInfoTranslator. Maybe a corresponding plugin is missing.";
        throw exceptions::InvalidConfiguration(msg);
    }
    return create();
}

void
Translator::register_factory(const string& key, const Translator::FactoryFunctionPtr factory)
{
    _factory_map[key] = factory;
}

unordered_map<string, Translator::FactoryFunctionPtr> Translator::_factory_map;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::translator
