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
