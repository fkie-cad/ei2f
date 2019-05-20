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

#include "StdUnorderedSet.h"

#include <iostream>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;

/****************************************************
 * class StdUnorderedSet::ReadIterator              *
 ****************************************************/

StdUnorderedSet::ReadIterator::ReadIterator(StdUnorderedSet& be): _pos(be._backend.cbegin()),
                                                                  _end(be._backend.cend()),
                                                                  _item_key(be.item_key())
{
}

bool
StdUnorderedSet::ReadIterator::next(ItemInfo& item_info)
{
    if (_pos == _end)
        return false;

    item_info.reset(_item_key, *_pos);

    ++_pos;
    return true;
}

StdUnorderedSet::ReadIterator::~ReadIterator()
{
    // INTENTIONALLY LEFT EMPTY
}


/****************************************************
 * class StdUnorderedSet                            *
 ****************************************************/

StdUnorderedSet::StdUnorderedSet(const ObjectConfig& config) : Backend(config)
{
    // INTENTIONALLY LEFT EMPTY
}

StdUnorderedSet::~StdUnorderedSet()
{
    // INTENTIONALLY LEFT EMPTY
}

ContainDecision
StdUnorderedSet::contains(ItemInfo& item_info)
{
    string& item = item_info.get(item_key());
    return _backend.count(item) > 0 ? contained : not_contained;
}

Backend *
StdUnorderedSet::create_from_config(const ObjectConfig& config) {
    return new StdUnorderedSet(config);
}

void
StdUnorderedSet::destroy(void)
{
    _backend.clear();
}

ItemAPI&
StdUnorderedSet::item_api(void)
{
    return *this;
}

/****************************************************
 * class StdUnorderedSet (part ItemAPI)             *
 ****************************************************/

ItemAPI::ReadIterator*
StdUnorderedSet::create_read_iterator(void)
{
    return new ReadIterator(*this);
}

void
StdUnorderedSet::add(ItemInfo& item_info)
{
    string& item = item_info.get(item_key());
    _backend.insert(item);
}

void
StdUnorderedSet::remove(ItemInfo& item_info)
{
    string& item = item_info.get(item_key());
    _backend.erase(item);
}

void
StdUnorderedSet::clear(void)
{
    _backend.clear();
}


/****************************************************
 * Plugin loading                                   *
 ****************************************************/

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::backend::StdUnorderedSet: Initializing Plugin" << endl;
    Backend::register_factory("StdUnorderedSet", StdUnorderedSet::create_from_config);
    Backend::register_factory("de::fraunhofer::fkie::ei2f::backend::StdUnorderedSet", StdUnorderedSet::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend
