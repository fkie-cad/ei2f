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
#include <unordered_map>
#include <set>
#include <assert.h>

#include <ei2f/helper.h>

using namespace std;

using namespace de::fraunhofer::fkie::ei2f;
void test_string_replace_all_by_dict(void)
{
    cout << "Running " << __FUNCTION__ << endl;

    std::unordered_map<string,string> mapping;
    mapping["t1"] = "r1";
    mapping["test2"] = "r2";
    mapping["t3"] = "replaced by 3";

    string sut      = "This is my {test2} test2, for longer {t3} or the same {t3}, it might {test2}. It is illegal to modify {test2 } or { t1} or even {t3";
    string expected = "This is my r2 test2, for longer replaced by 3 or the same replaced by 3, it might r2. It is illegal to modify {test2 } or { t1} or even {t3";

    replace_all_by_dict(sut, mapping, "{", "}");
    assert(sut == expected);
}

void test_string_replace_all(void)
{
    cout << "Running " << __FUNCTION__ << endl;

    string sut      = "This is the ultimate test. More testing has never be done.";
    string expected = "This is the ultimate music. More musicing has never be done.";

    replace_all(sut, "test", "music");
    assert(sut == expected);

    expected = "That is the ultimate music. More musicing has never be done.";
    replace_all(sut, "This", "That");
    assert(sut == expected);

    expected = "That is the ultimate music. More musicing has bad influence on health.";
    replace_all(sut, "never be done", "bad influence on health");
    assert(sut == expected);
}

int
main(int argc, char **argv)
{
    test_string_replace_all();
    test_string_replace_all_by_dict();
    return 0;
}
