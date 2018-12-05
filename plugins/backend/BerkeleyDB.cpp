#include <vector>
#include <iostream>
#include <string>
#include <cstring>

#include "BerkeleyDB.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;


/****************************************************
 * class BerkeleyDB::ReadIterator                   *
 ****************************************************/

BerkeleyDB::ReadIterator::ReadIterator(BerkeleyDB& db) : _dbc(NULL), _item_key(db.item_key())
{
    int ret = db._db->cursor(db._db, NULL, &_dbc, 0);
    if (ret != 0) {
        close();
        // TODO More specific exception
        throw exception();
    }
}

bool
BerkeleyDB::ReadIterator::next(ItemInfo& item_info)
{
    char value_placeholder;
    DBT key, value;

    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));

    value.data = &value_placeholder;
    value.ulen = 0;
    value.flags = DB_DBT_USERMEM;

    int ret = _dbc->get(_dbc, &key, &value, DB_NEXT);
    if (ret == 0) {
        // No error
        string s((const char*)key.data, key.size);
        item_info.reset(_item_key, s);
        return true;
    } else if (ret == DB_NOTFOUND) {
        // End of data
        return false;
    } else {
        // TODO More specific exception
        throw exception();
    }
}

void
BerkeleyDB::ReadIterator::close(void)
{
    if (_dbc) {
        _dbc->close(_dbc);
        // Also called from destructor --> No error throwing possible here
        _dbc = NULL;
    }
}

BerkeleyDB::ReadIterator::~ReadIterator()
{
    close();
}


/****************************************************
 * class BerkeleyDB                                 *
 ****************************************************/

BerkeleyDB::BerkeleyDB(const ObjectConfig& config) : Backend(config), PathMixin(config, "Backend.Path"), _db(NULL)
{
    std::unordered_map <string, DBTYPE> type_lookup;
    type_lookup["Hash"] = DB_HASH;
    type_lookup["BTree"] = DB_BTREE;
    try {
        _db_type = type_lookup.at(config.at("BerkeleyDB.Type"));
    } catch (out_of_range& e) {
        _db_type = DB_BTREE;
    }

    if (path() == "") {
        _path = NULL;
        _name = NULL;
    } else {
        _path = path().c_str();
        _name = "main";
    }

    open();
}

void
BerkeleyDB::close(void)
{
    if (_db != NULL) {
        _db->close(_db, 0);
        _db = NULL;
    }
}

void
BerkeleyDB::open(void)
{
    if (_db == NULL) {
        int ret;

        /* setup the database memory structure */
        ret = db_create(&_db, NULL, 0);

        if (ret != 0) {
            close();
            // TODO More specific exception
            throw exception();
        }

        ret = _db->open(
                    _db,
                    NULL,
                    _path,
                    _name,
                    _db_type,
                    DB_CREATE,
                    0);

        if (ret != 0) {
            close();
            // TODO More specific exception
            throw exception();
        }
    }
}

BerkeleyDB::~BerkeleyDB()
{
    close();
}

ContainDecision
BerkeleyDB::contains(ItemInfo& item_info)
{
    string& item = item_info.get(item_key());

    char value_placeholder;
    DBT key, value;

    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));

    key.data = const_cast<char*>(item.c_str());
    key.size = item.size();

    value.data = &value_placeholder;
    value.ulen = 0;
    value.flags = DB_DBT_USERMEM;

    int ret = _db->get(_db,
                        NULL,
                        &key,
                        &value,
                        0);
    if (ret == 0) {
        return contained;
    } else if (ret == DB_NOTFOUND) {
        return not_contained;
    } else {
        // TODO More specific exception
        throw exception();
    }
}

void
BerkeleyDB::destroy(void)
{
    // Possibly FIXME ERROR HANDLING

    // First close out handle
    close();
    // _db is not usable anymore, so we need a new temporary structure
    // that we create with db_create
    DB* tmp_db;
    db_create(&tmp_db, NULL, 0);

    // This will delete the db file and close the temporary handle
    tmp_db->remove(tmp_db, _path, NULL, 0);
}

Backend *
BerkeleyDB::create_from_config(const ObjectConfig& config) {
    return new BerkeleyDB(config);
}

ItemAPI&
BerkeleyDB::item_api(void)
{
    return *this;
}


/****************************************************
 * class BerkeleyDB (part ItemAPI)                  *
 ****************************************************/

ItemAPI::ReadIterator*
BerkeleyDB::create_read_iterator(void)
{
    return new ReadIterator(*this);
}

void
BerkeleyDB::add(ItemInfo& item_info)
{
//    if (contains(domain) == contained)
//        return;
    string& item = item_info.get(item_key());

    char value_placeholder = 1;
    DBT key, value;

    memset(&key, 0, sizeof(DBT));
    memset(&value, 0, sizeof(DBT));

    /* setup the key */
    key.data = const_cast<char*>(item.c_str());
    key.size = item.size();

    /* setup the value */
    value.data = &value_placeholder;
    value.size = 0;

    /* write it into the database */

    _db->put(_db,
             NULL,
             &key,
             &value,
             DB_NOOVERWRITE);
    // Possible FIXME: If needed add resulting error exeception
}

void
BerkeleyDB::remove(ItemInfo& item_info)
{
    string& item = item_info.get(item_key());

    DBT key;

    memset(&key, 0, sizeof(DBT));

    /* setup the key */
    key.data = const_cast<char*>(item.c_str());
    key.size = item.size();

    /* write it into the database */
    _db->del(_db,
             NULL,
             &key,
             0);

    // Possible FIXME: If needed add resulting error exeception
}

void
BerkeleyDB::clear(void)
{
    // First close handle
    close();

    // _db is not usable anymore, so we need a new temporary structure
    // that we create with db_create
    DB* tmp_db;
    db_create(&tmp_db, NULL, 0);

    // This will NOT delete the db file, only the db "main" inside the file and close the temporary handle
    tmp_db->remove(tmp_db, _path, _name, 0);

    open();
}


/****************************************************
 * Plugin loading                                   *
 ****************************************************/

void on_library_load(void) __attribute__((constructor));

void
on_library_load(void)
{
    cout << "de::fraunhofer::fkie::ei2f::backend::BerkeleyDB: Initializing Plugin" << endl;
    Backend::register_factory("BerkeleyDB", BerkeleyDB::create_from_config);
    Backend::register_factory("de::fraunhofer::fkie::ei2f::backend::BerkeleyDB", BerkeleyDB::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend
