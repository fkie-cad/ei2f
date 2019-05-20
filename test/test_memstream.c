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
#include <string>

#include <ei2f/helper.h>

using namespace std;

int main(int argc, char **argv)
{
    char buf[1024];
    for (int i = 0; i < 1024; ++i) {
        buf[i] = 0x11;
    }
    OMemoryStream oms(buf, 1);
    ifstream fs("./somedata");
    char c;
    int count = 0;
    for (;count<5;++count) {
        fs.get(c);
    }
//    oms << fs.rdbuf();

    for (int i = 0; i < 1024; i=i+8) {
        for (int offset=0; offset < 8; ++offset) {
            printf("%2x ", buf[i+offset]);
        }
        printf("\n");
    }
//    cout << fs.rdbuf();
    cout << fs.eof() << endl;
    cout << oms.eof() << endl;
}
