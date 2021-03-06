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
#include <string>
#include <assert.h>
#include <ctime>
#include <openssl/sha.h>
#include <string.h>

// for registering factories
#include <ei2f/ListPipeline.h>
#include <ei2f/Stage.h>
#include <ei2f/backend/BinaryAPI.h>
#include <ei2f/backend/ItemAPI.h>
#include <ei2f/pluginloader.h>

using namespace std;

using namespace de::fraunhofer::fkie::ei2f;
using namespace de::fraunhofer::fkie::ei2f::backend;

class PerfBloomStage: public Stage {
    public:
        PerfBloomStage(const ObjectConfig& config) : Stage(config) {
            update_bloom();
        }

        ~PerfBloomStage(void) {}

        void update_bloom(void) {
    	{
            BackendPtr be = this->backend();
            BinaryAPI& api = be->binary_api();
            BinaryAPI::BLOB blob;
            api.binary_ref(blob);
            for(unsigned int i = 0; i < blob.size; ++i) {
                blob.data[i] = 0xff;
            }
        }
    }
};

class PerfStdUnorderedSetStage: public Stage {
    public:
        PerfStdUnorderedSetStage(const ObjectConfig& config) : Stage(config) {
            update_set();
        }

        ~PerfStdUnorderedSetStage(void) {}

        void update_set(void) {
    	{
            BackendPtr be = this->backend();
            ItemAPI& api = be->item_api();
            unsigned long long buf = 0x1deadbeef;
            assert(sizeof(buf) == 8);
            ItemInfo info;
            for(unsigned long long offset = 0; offset < 50; ++offset, ++buf) {
                string data((char*)(void*)&buf, 8);
                info.reset("Item", data);
                api.add(info);
            }
        }
    }
};

class TestListPipeline : public ListPipeline {
    public:
        TestListPipeline(const string& config_path) : ListPipeline(config_path) { }

        void add_test_stage(StagePtr stage) {
            add_stage(stage);
        }
};

StagePtr create_bloom_stage(unsigned id, unsigned int bit_offset) {
    ObjectConfig config;
//    config["Stage.Dir"]="./listspool/test2/";
    config["Stage.PersistenceLevel"]="None";
    config["Stage.Name"]=std::to_string(id);
    config["Backend.Type"]="BloomFilter";
    config["Backend.Path"]="";
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
    config["Backend.ItemKey"]="SHA256.Binary";
    config["Backend.HashBits"]="22";
    config["Backend.HashBitsOffset"]= std::to_string(bit_offset);
    StagePtr result(new PerfBloomStage(config));
    return result;
}

StagePtr create_nohash_bloom_stage(unsigned id, unsigned int bit_offset) {
    ObjectConfig config;
//    config["Stage.Dir"]="./listspool/test2/";
    config["Stage.PersistenceLevel"]="None";
    config["Stage.Name"]=std::to_string(id);
    config["Backend.Type"]="BloomFilter";
    config["Backend.Path"]="";
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
//    config["Backend.ItemKey"]="SHA256.Binary";
    config["Backend.ItemKey"]="Item";
    config["Backend.HashBits"]="22";
    config["Backend.HashBitsOffset"]= std::to_string(bit_offset);
    StagePtr result(new PerfBloomStage(config));
    return result;
}

StagePtr create_stdunorderedset_stage(unsigned id) {
    ObjectConfig config;
//    config["Stage.Dir"]="./listspool/test2/";
    config["Stage.PersistenceLevel"]="None";
    config["Stage.Name"]=std::to_string(id);
    config["Backend.Type"]="StdUnorderedSet";
    config["Backend.Path"]="";
    config["Backend.UUID"]="00000000-0000-0000-0000-000000000000";
    config["Backend.ItemKey"]="Item";
    StagePtr result(new PerfStdUnorderedSetStage(config));
    return result;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << "<testcases>" << endl;
        return -1;
    }
    int testcases = std::stoi(argv[1]);
    int testcase;

    if(system("mkdir -p /tmp/abcdefghijklm")) {}
    else {}

    clock_t begin;
    clock_t end;

    // load all plugins
    load_plugins();

    // Raw sha256 bench
    unsigned char hash[SHA256_DIGEST_LENGTH];
    begin = clock();

    for(int testcase = 0 ; testcase < testcases; ++testcase) {
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, "BLABLABLA", 9);
        SHA256_Final(hash, &sha256);
    }
    end = clock();
    double elapsed = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Raw SHA256 in ns:                             " << elapsed * 1000000000.0 / testcases << endl;


    // sha256 in iteminfo bench
    ItemInfo item;
    string str("BLABLALBA");
    begin = clock();

    for(int testcase = 0 ; testcase < testcases; ++testcase) {
        item.reset("Item", str);
        item.get("SHA256.Binary");
    }
    end = clock();
    elapsed = double(end - begin) / CLOCKS_PER_SEC;
    cout << "SHA256 in ItemInfo in ns:                     " << elapsed * 1000000000.0 / testcases << endl;

    unsigned int stages = 8;

    // Create a bloomfilter only pipeline
    std::shared_ptr<TestListPipeline> pipe(new TestListPipeline("/tmp/abcdefghijklm"));

    // For now only add one stage
    pipe->add_test_stage(create_bloom_stage(0, 0));

    // measure response times for single stage bloomfilter pipeline start ...
    begin = clock();

    for (testcase = 0; testcase < testcases; ++testcase) {
        assert(pipe->contains("BLABLABLA") == false);
    }


    // .. end measure response times for single stage bloomfilter pipeline
    end = clock();
    elapsed = double(end - begin) / CLOCKS_PER_SEC;

    cout << "BloomFilter: First stage in ns:               " << elapsed * 1000000000.0 / testcases << endl;

    // bloomfilter stage to measure secondary bloomfilter timings
    pipe.reset(new TestListPipeline("/tmp/abcdefghijklm"));
    pipe->add_test_stage(create_nohash_bloom_stage(0, 0));
    char hash_buf[32];
    memset(hash_buf,0,32);
    string hashed_data(hash_buf,32);

    // start measure ...
    begin = clock();

    for (testcase = 0; testcase < testcases; ++testcase) {
        assert(pipe->contains(hashed_data) == false);
    }

    // ... end measure
    end = clock();
    elapsed = double(end - begin) / CLOCKS_PER_SEC;

    cout << "BloomFilter: Other stages per stage in ns:    " << elapsed * 1000000000.0 / testcases << endl;

    // Create a new pipeline for tests with StdUnorderedSet
    // (populated with 50 8 byte items non zero terminated strings
    pipe.reset(new TestListPipeline("/tmp/abcdefghijklm"));
    pipe->add_test_stage(create_stdunorderedset_stage(0));

    // First checking timing of an item that is contained
    unsigned long long buf = 0x1deadbeef;
    string data((char*)(void*)&buf, 8);
    testcases *= 10;

    begin = clock();

    for (int testcase = 0; testcase < testcases; ++testcase) {
        assert(pipe->contains(data) == true);
    }

    end = clock();
    double elapsed_set_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "StdUnorderedSet: Single stage match in ns:    " << (elapsed_set_secs) * 1000000000.0 / testcases << endl;

    // Then checking timing of an item that is not contained
    buf = 0xffffffffffff;
    string data2((char*)(void*)&buf, 8);

    begin = clock();

    for (int testcase = 0; testcase < testcases; ++testcase) {
        assert(pipe->contains(data2) == false);
    }

    end = clock();
    elapsed_set_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "StdUnorderedSet: Single stage no match in ns: " << (elapsed_set_secs) * 1000000000.0 / testcases << endl;
}
