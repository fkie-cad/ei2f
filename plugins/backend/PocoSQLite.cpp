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

#include <vector>
#include <iostream>
#include <stdexcept>

#include "Poco/File.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/SQLiteException.h"

#include "PocoSQLite.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::SQLite::Connector;

/****************************************************
 * class PocoSQLite::ReadIterator                   *
 ****************************************************/

PocoSQLite::ReadIterator::ReadIterator(PocoSQLite& backend) : _record_set(*(backend._session), "SELECT item FROM items"),
                                                              _pos(_record_set.begin()),
                                                              _end(_record_set.end())
{
    _item_key = backend.item_key();
}

bool
PocoSQLite::ReadIterator::next(ItemInfo& item_info)
{
    if (_pos == _end)
        return false;

    item_info.reset(_item_key, (*_pos)[0].toString());

    ++_pos;
    return true;
}

PocoSQLite::ReadIterator::~ReadIterator()
{
    // INTENTIONALLY LEFT EMPTY
}


/****************************************************
 * class PocoSQLite                                 *
 ****************************************************/

PocoSQLite::PocoSQLite(const ObjectConfig& config) : Backend(config), PathMixin(config, "Backend.Path")
{
    Connector::registerConnector();

    // create a session
    string connection_string;
    if (path() == "") {
        connection_string = ":memory:";
    } else {
        connection_string = path();
    }
    _session = new Session("SQLite", connection_string);

    *_session << "CREATE TABLE IF NOT EXISTS items (item VARCHAR(256) PRIMARY KEY)", now;
    _add_statement = new Statement((*_session << "INSERT OR REPLACE INTO items VALUES(:data)", use(_item)));
    _contains_statement = new Statement((*_session << "SELECT COUNT(*) FROM items WHERE item=(:data)", use(_item), into(_count)));
    _remove_statement = new Statement((*_session << "DELETE FROM items WHERE item=(:data)", use(_item)));
}

PocoSQLite::~PocoSQLite()
{
    if (!_session.isNull()) {
        _session->close();
        _session = NULL;
    }
}

ContainDecision
PocoSQLite::contains(ItemInfo& item_info)
{
    _item = item_info.get(item_key());
    _contains_statement->execute();
    return _count > 0 ? contained : not_contained;
}

void
PocoSQLite::destroy(void)
{
    if (!_session.isNull()) {
        _session->close();
        _session = NULL;
    }
    if (path() != "") {
        // Not in memory only
        Poco::File f(path());
        f.remove();
    }
}

ItemAPI&
PocoSQLite::item_api(void)
{
    return *this;
}

Backend *
PocoSQLite::create_from_config(const ObjectConfig& config) {
    return new PocoSQLite(config);
}


/****************************************************
 * class PocoSQLite (part ItemAPI                   *
 ****************************************************/

ItemAPI::ReadIterator*
PocoSQLite::create_read_iterator(void)
{
    return new ReadIterator(*this);
}

void
PocoSQLite::add(ItemInfo& item_info)
{
    try {
        _item = item_info.get(item_key());
        _add_statement->execute();
    } catch (Poco::Data::SQLite::ConstraintViolationException& e) {}
}

void
PocoSQLite::remove(ItemInfo& item_info)
{
    try {
        _item = item_info.get(item_key());
        _remove_statement->execute();
    } catch (Poco::Data::SQLite::ConstraintViolationException& e) {}
}

void
PocoSQLite::clear(void)
{
    *_session << "DROP TABLE IF EXISTS items", now;
    *_session << "CREATE TABLE IF NOT EXISTS items (item VARCHAR(256) PRIMARY KEY)", now;
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
    cout << "de::fraunhofer::fkie::ei2f::backend::PocoSQLite: Initializing Plugin" << endl;
    Backend::register_factory("PocoSQLite", PocoSQLite::create_from_config);
    Backend::register_factory("de::fraunhofer::fkie::ei2f::backend::PocoSQLite", PocoSQLite::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend
