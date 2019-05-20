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
