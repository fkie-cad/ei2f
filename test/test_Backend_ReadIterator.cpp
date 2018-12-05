#include <iostream>
#include <string>
#include <set>
#include <assert.h>

#include <ei2f/common.h>
#include <ei2f/pluginloader.h>
#include <ei2f/ItemInfo.h>
#include <ei2f/backend/ItemAPI.h>
#include <ei2f/backend/Backend.h>

using namespace std;

using de::fraunhofer::fkie::ei2f::ObjectConfig;
using de::fraunhofer::fkie::ei2f::ItemInfo;
using de::fraunhofer::fkie::ei2f::load_plugins;
using de::fraunhofer::fkie::ei2f::ContainDecision;

using de::fraunhofer::fkie::ei2f::backend::Backend;
using de::fraunhofer::fkie::ei2f::backend::BackendPtr;
using de::fraunhofer::fkie::ei2f::backend::ItemAPI;

typedef std::shared_ptr<ItemAPI::ReadIterator> ReadIteratorPtr;

void test_ReadIterator(const string& backend_type, const string& item_key)
{
    ObjectConfig config;
    config["Backend.Type"]=backend_type;
    config["Backend.ItemKey"]=item_key;
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
    config["Backend.Path"]="";
    config["Updater.Type"]="None";

    std::set<string> original_items;
    original_items.insert("test1.de");
    original_items.insert("test2.de");
    original_items.insert("test3.de");

    BackendPtr dut(Backend::create_from_config(config));
    ItemAPI& dut_api = dut->item_api();
    ItemInfo item_info;

    for (auto item: original_items) {
        item_info.reset(item_key, item);
        dut_api.add(item_info);
    }

    std::set<string> retrieved_items;

    ReadIteratorPtr iter(dut_api.create_read_iterator());

    item_info.reset();

    while (iter->next(item_info)) {
        retrieved_items.insert(item_info.get(item_key));
    }

    assert(retrieved_items == original_items);
}

void test_StdUnorderedSet_ReadIterator_on_item_key_Item(void)
{
    cout << "Running " << __FUNCTION__ << endl;
    test_ReadIterator("StdUnorderedSet", "Item");
}

void test_StdUnorderedSet_ReadIterator_on_item_key_Other(void)
{
    cout << "Running " << __FUNCTION__ << endl;
    test_ReadIterator("StdUnorderedSet", "Other");
}

void test_PocoSQLite_ReadIterator_on_item_key_Item(void)
{
    cout << "Running " << __FUNCTION__ << endl;
    test_ReadIterator("PocoSQLite", "Item");
}

void test_PocoSQLite_ReadIterator_on_item_key_Other(void)
{
    cout << "Running " << __FUNCTION__ << endl;
    test_ReadIterator("PocoSQLite", "Other");
}

void test_BerkeleyDB_ReadIterator_on_item_key_Item(void)
{
    cout << "Running " << __FUNCTION__ << endl;
    test_ReadIterator("BerkeleyDB", "Item");
}

void test_BerkeleyDB_ReadIterator_on_item_key_Other(void)
{
    cout << "Running " << __FUNCTION__ << endl;
    test_ReadIterator("BerkeleyDB", "Other");
}

int
main(int argc, char **argv)
{
    // load all plugins
    load_plugins();

    test_StdUnorderedSet_ReadIterator_on_item_key_Item();
    test_StdUnorderedSet_ReadIterator_on_item_key_Other();
    test_PocoSQLite_ReadIterator_on_item_key_Item();
    test_PocoSQLite_ReadIterator_on_item_key_Other();
    test_BerkeleyDB_ReadIterator_on_item_key_Item();
    test_BerkeleyDB_ReadIterator_on_item_key_Other();

    return 0;
}
