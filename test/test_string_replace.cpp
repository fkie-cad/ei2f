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
