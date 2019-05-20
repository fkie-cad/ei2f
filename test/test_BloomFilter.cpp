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

#include <assert.h>
#include <iostream>
#include <string>

// for registering factories
#include <ei2f/common.h>
#include <ei2f/ItemInfo.h>
#include <ei2f/ListPipeline.h>
#include <ei2f/pluginloader.h>
#include <ei2f/backend/Backend.h>
#include <ei2f/backend/BinaryAPI.h>

using namespace std;

using de::fraunhofer::fkie::ei2f::ListPipeline;
using de::fraunhofer::fkie::ei2f::ObjectConfig;
using de::fraunhofer::fkie::ei2f::backend::Backend;
using de::fraunhofer::fkie::ei2f::backend::BackendPtr;
using de::fraunhofer::fkie::ei2f::backend::BinaryAPI;
using de::fraunhofer::fkie::ei2f::ItemInfo;
using de::fraunhofer::fkie::ei2f::load_plugins;
using de::fraunhofer::fkie::ei2f::ContainDecision;

void
fill_item_info(ItemInfo& item_info, const string& domain, const string& hash_key, char* raw_hash, unsigned int hash_len)
{
    item_info.set(string("Item"), domain);
    std::vector<char> binary;
    for (unsigned int i = 0; i < hash_len; ++i) {
        binary.push_back(raw_hash[i]);
    }
    item_info.set(hash_key, string(binary.begin(), binary.end()));
}

void test_BloomFilter1(void)
{
    cout << "Running " << __FUNCTION__ << endl;

    ObjectConfig config;
//    config["Stage.Dir"]="./listspool/test2/";
    config["Backend.Type"]="BloomFilter";
    config["Backend.Path"]="";
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
    config["Backend.ItemKey"]="DUMMY.Binary";
    config["Backend.HashBits"]="4";
/*    config["Updater.Type"]="Binary";
    config["Updater.InPlace"]="true";
    config["Updater.Uri"]="./import/testbloom";
*/
    string hash_item_key("DUMMY.Binary");

    BackendPtr bloom(Backend::create_from_config(config));
    BinaryAPI& bloom_api = bloom->binary_api();

    // Set bloom filter data
    BinaryAPI::BLOB blob;
    bloom_api.binary_ref(blob);

    assert(blob.size == 2);
    blob.data[0] = 0x55;
    blob.data[1] = 0x55;

    char hash[1];
    hash[0] = 0x00;

    for(int i = 0; i < 16; ++i) {
        ItemInfo item_info;
        fill_item_info(item_info, string("test.de"), hash_item_key, hash, 1);
        ContainDecision result = bloom->contains(item_info);
        if (i%2 == 0) {
            assert(result == ContainDecision::undecided);
        } else {
            assert(result == ContainDecision::not_contained);
        }
        hash[0]++;
    }
}

void test_BloomFilter2(void)
{
    cout << "Running " << __FUNCTION__ << endl;

    ObjectConfig config;
//    config["Stage.Dir"]="./listspool/test2/";
    config["Backend.Type"]="BloomFilter";
    config["Backend.Path"]="";
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
    config["Backend.ItemKey"]="DUMMY.Binary";
    config["Backend.HashBits"]="5";
    // --> blob.size == 2**5 == 32 bits == 4 bytes
    config["Backend.Rounds"]="2";
    // --> 10 bit hash needed --> 2 chars
/*    config["Updater.Type"]="Binary";
    config["Updater.InPlace"]="true";
    config["Updater.Uri"]="./import/testbloom";
*/
    string hash_item_key("DUMMY.Binary");

    BackendPtr bloom(Backend::create_from_config(config));
    BinaryAPI& bloom_api = bloom->binary_api();

    // Set bloom filter data
    BinaryAPI::BLOB blob;
    bloom_api.binary_ref(blob);

    assert(blob.size == 4);
    blob.data[0] = 0x55;
    blob.data[1] = 0x55;
    blob.data[2] = 0x55;
    blob.data[3] = 0x55;

    char hash[2];

    for(int l = 0; l < 7; ++l) {
    for(int h = 16; h < 32; ++h) {
        ItemInfo item_info;
        unsigned long lhash = l + (h << 5);
        hash[0] = lhash & 0xff;
        hash[1] = (lhash & 0xff00) >> 8;
        fill_item_info(item_info, string("test.de"), hash_item_key, hash, 2);
        ContainDecision result = bloom->contains(item_info);
        if (h%2 == 0 && l%2 == 0) {
            assert(result == ContainDecision::undecided);
        } else {
            assert(result == ContainDecision::not_contained);
        }
    }}
}

void test_BloomFilter3(void)
{
    cout << "Running " << __FUNCTION__ << endl;

    ObjectConfig config;
//    config["Stage.Dir"]="./listspool/test2/";
    config["Backend.Type"]="BloomFilter";
    config["Backend.Path"]="";
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
    config["Backend.ItemKey"]="DUMMY.Binary";
    config["Backend.HashBits"]="3";
    // --> blob.size == 2**3 == 8 bits == 1 byte
    config["Backend.Rounds"]="3";
    // --> 8*3 = 24 bit hash needed --> 3 chars
/*    config["Updater.Type"]="Binary";
    config["Updater.InPlace"]="true";
    config["Updater.Uri"]="./import/testbloom";
*/

    string hash_item_key("DUMMY.Binary");

    BackendPtr bloom(Backend::create_from_config(config));
    BinaryAPI& bloom_api = bloom->binary_api();

    // Set bloom filter data
    BinaryAPI::BLOB blob;
    bloom_api.binary_ref(blob);

    assert(blob.size == 1);
    blob.data[0] = 0x81;

    char hash[3];

    for(int l = 0; l < 8; ++l) {
    for(int m = 0; m < 8; ++m) {
    for(int h = 0; h < 8; ++h) {
        ItemInfo item_info;
        unsigned long lhash = l + (m << 3) + (h << 6);
        hash[0] = lhash & 0xff;
        hash[1] = (lhash & 0xff00) >> 8;
        hash[2] = (lhash & 0xff0000) >> 16;
        fill_item_info(item_info, string("test.de"), hash_item_key, hash, 3);
        ContainDecision result = bloom->contains(item_info);
        if ((h==0 || h==7) && (m==0 || m==7) && (l==0 || l==7)) {
            assert(result == ContainDecision::undecided);
        } else {
            assert(result == ContainDecision::not_contained);
        }
    }}}
}

int
main(int argc, char **argv)
{
    // load all plugins
    load_plugins();

    test_BloomFilter1();
    test_BloomFilter2();
    test_BloomFilter3();
    return 0;
}
