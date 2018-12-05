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
