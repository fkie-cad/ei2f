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

#include <ei2f/common.h>
#include <ei2f/ListPipeline.h>
#include <ei2f/pluginloader.h>
#include <ei2f/ItemInfo.h>
#include <ei2f/backend/Backend.h>

using namespace std;

using de::fraunhofer::fkie::ei2f::ListPipeline;
using de::fraunhofer::fkie::ei2f::ObjectConfig;
using de::fraunhofer::fkie::ei2f::ItemInfo;
using de::fraunhofer::fkie::ei2f::load_plugins;
using de::fraunhofer::fkie::ei2f::ContainDecision;

using de::fraunhofer::fkie::ei2f::backend::Backend;
using de::fraunhofer::fkie::ei2f::backend::BackendPtr;

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

void test_Online_1(void)
{
    cout << "Running " << __FUNCTION__ << endl;

    ObjectConfig config;
    config["Stage.Dir"]="./listspool/test2/";
    config["Backend.Type"]="Online";
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
    config["Backend.ItemKey"]="SHA256.Hex";
    config["Backend.CompareItemKey"]="DUMMY.Hex";
    config["Backend.UriPattern"]="http://localhost:4711/v1/{Key}";
    config["TransportType"]="file";

    BackendPtr backend(Backend::create_from_config(config));

    ItemInfo item_info("Item", "test1.de");
    backend->contains(item_info);
}

int
main(int argc, char **argv)
{
    // load all plugins
    load_plugins();

    test_Online_1();

    return 0;
}
