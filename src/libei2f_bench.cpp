#include <iostream>
#include <string>
#include <assert.h>
#include <ctime>

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
            for(unsigned long long offset = 0; offset < 600000; ++offset, ++buf) {
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
    if(system("mkdir -p /tmp/abcdefghijklm")) {}
    else {}

    // load all plugins
    load_plugins();

    unsigned int stages = 8;

    // Create a bloomfilter only pipeline
    std::shared_ptr<TestListPipeline> pipe(new TestListPipeline("/tmp/abcdefghijklm"));

    // For now only add one stage
    pipe->add_test_stage(create_bloom_stage(0, 0));
    int testcases = std::stoi(argv[1]);

    // measure response times for single stage bloomfilter pipeline start ...
    clock_t begin = clock();

    for (int testcase = 0; testcase < testcases; ++testcase) {
        assert(pipe->contains("BLABLABLA") == false);
    }


    // .. end measure response times for single stage bloomfilter pipeline
    clock_t end = clock();
    double elapsed1_secs = double(end - begin) / CLOCKS_PER_SEC;

    // adding 7 more bloomfilter stages to measure secondary bloomfilter timings
    for (unsigned int i = 1; i < stages; ++i) {
        pipe->add_test_stage(create_bloom_stage(i, i*22));
    }

    // start measure ...
    begin = clock();

    for (int testcase = 0; testcase < testcases; ++testcase) {
        assert(pipe->contains("BLABLABLA") == false);
    }

    // ... end measure
    end = clock();
    double elapsed8_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << "BloomFilter: First stage in ns:               " << elapsed1_secs * 1000000000.0 / testcases << endl;
    cout << "BloomFilter: Other stages per stage in ns:    " << (elapsed8_secs-elapsed1_secs) * 1000000000.0 / (7 * testcases) << endl;

    // Create a new pipeline for tests with StdUnorderedSet
    // (populated with 600000 8 byte items non zero terminated strings
    pipe.reset(new TestListPipeline("/tmp/abcdefghijklm"));
    pipe->add_test_stage(create_stdunorderedset_stage(0));

    // First checking timing of an item that is contained
    unsigned long long buf = 0x1deadffff;
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
