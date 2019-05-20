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
