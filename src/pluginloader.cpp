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
    std::vector<std::string> directories;
    directories.push_back("/usr/local/lib/libei2f/");
    directories.push_back("/usr/lib/libei2f/");

    // Only for in build tree runs
    directories.push_back("./.libs/");

    for (auto directory: directories) {
        try {
            read_directory(directory, files);
        } catch (std::runtime_error &e) {
            // Directory not found
            continue;
        }
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
            dlerror();
            void * handle = dlopen(file.c_str(), RTLD_NOW);
            if (!handle)
                cout << dlerror() << endl;
            dlerror();

            char * error = NULL;
            void (*init_ei2f_plugin)(void);

            *(void**) (&init_ei2f_plugin) = dlsym(handle, "init_ei2f_plugin");
            error = dlerror();
            if (error) {
                cout << error << endl;
            } else {
                init_ei2f_plugin();
            }
        }
    }
}

}}}} // Namespace de::fraunhofer::fkie::ei2f
