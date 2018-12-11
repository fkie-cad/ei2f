#include <iostream>
#include <fstream>
#include <algorithm>

#include <ei2f/common.h>
#include <ei2f/helper.h>
#include <ei2f/ListPipeline.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using namespace std;

using ei2f::ObjectConfig;
using ei2f::ItemInfo;

ListPipeline::ListPipeline(const string& path)
{
    vector<string> filenames;
    read_directory(path, filenames);
    std::sort(filenames.begin(), filenames.end());
    for (auto filename: filenames) {
        if (ends_with(filename, ".stage")) {
            string stagename = filename.substr(0, filename.length() - 5);
            if (stagename.length() == 0) {
                continue;
            }
            string filepath = path + "/" + filename;
            cout << "Found Stage config " << filepath << endl;
            ObjectConfig config;
            config["Stage.Name"] = stagename;
            ifstream input;
            input.open(filepath);
            string delim = "=";
            string line;

            while (getline(input, line)) {
                string key;
                string value;
                left_split(line, delim, key, value);
                cout << key << " " << value << endl;
                config[key] = value;
            }
            StagePtr dl(Stage::create_from_config(config));
            dl->try_update();
            add_stage(dl);
        }
    }
}

void
ListPipeline::add_stage(StagePtr domain_stage)
{
    _stages.push_back(domain_stage);
}

void
ListPipeline::clear_stages(void)
{
    _stages.clear();
}

bool
ListPipeline::contains(const string& item)
{
    ItemInfo item_info;
    item_info.set("Item", item);
//    cout << "Item: " << item  << endl;

    for (auto stage: _stages) {
        switch (stage->contains(item_info)) {
            case ei2f::contained:
//                cout << "CONTAINED" << endl;
                return true;
            case ei2f::not_contained:
//                cout << "NOT CONTAINED" << endl;
                return false;
            case ei2f::undecided:
//                cout << "UNDECIDED" << endl;
            default:
                continue;
        }
    }
    return false;
}

}}}} // Namespace de::fraunhofer::fkie::ei2f
