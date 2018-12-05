#include <set>
#include <iostream>
#include <string>
#include <vector>

#include <dlfcn.h>
#include <Poco/Glob.h>

#include <sys/types.h>
#include <dirent.h>

#include <ei2f/helper.h>
#include <ei2f/pluginloader.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using namespace std;

void load_plugins(void)
{
    std::vector<std::string> files;
    read_directory("./bin/plugins/", files);
    std::vector<std::string> loadable_files;

    cout << "de::fraunhofer::fkie::ei2f::load_plugins: Searching plugins" << endl;

    for (auto file: files) {
        if (ends_with(file, ".so")) {
            loadable_files.push_back(file);
            cout << "de::fraunhofer::fkie::ei2f::load_plugins: Found plugin '" << file << "'" << endl;
        }
    }

    cout << "de::fraunhofer::fkie::ei2f::load_plugins: Loading plugins" << endl;

    for (auto file: loadable_files) {
        if (!dlopen(file.c_str(), RTLD_NOW))
            cout << dlerror() << endl;
    }
}

}}}} // Namespace de::fraunhofer::fkie::ei2f
