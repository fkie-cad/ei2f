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

#include "File.h"
#include <iostream>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace transport {

using namespace std;

File::File(const ObjectConfig& config) : Transport(config)
{
}

File::~File()
{

}

istream&
File::uri_to_istream(const string& raw_uri)
{
    _file.close();
    cout << "Opening File " << raw_uri << endl;
    _file.open(raw_uri);
    return _file;
}

Transport *
File::create_from_config(const ObjectConfig& config) {
    return new File(config);
}

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::transport::File: Initilialing Plugin" << endl;
    Transport::register_factory("file", File::create_from_config);
    Transport::register_factory("", File::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::transport
