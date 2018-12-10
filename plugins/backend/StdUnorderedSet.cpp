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
