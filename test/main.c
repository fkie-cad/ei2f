#include <iostream>
#include <string>

// for registering factories
#include <ei2f/ListPipeline.h>
#include <ei2f/pluginloader.h>

using namespace std;

using de::fraunhofer::fkie::e2if::ListPipeline;
using de::fraunhofer::fkie::e2if::load_plugins;

int main(int argc, char **argv)
{
    // load all plugins
    load_plugins();

//    std::shared_ptr<DomainList> lists(new DomainList("./conf/lists/test1.list/"));
    std::shared_ptr<ListPipeline> lists(new ListPipeline("./conf/lists/test3.list/"));

    cout << lists->contains("09f728cdf8c4a1419f455ea347b5d05e.org") << endl;
    cout << lists->contains(std::string("b0815")) << endl;
    cout << lists->contains(std::string("a4711")) << endl;
    cout << lists->contains("4711") << endl;
    cout << lists->contains("4712") << endl;
    cout << lists->contains("4713") << endl;
    cout << lists->contains("4714") << endl;
    cout << lists->contains("4715") << endl;
    cout << lists->contains("4716") << endl;
    cout << lists->contains("4717") << endl;
    cout << lists->contains("4718") << endl;
    cout << lists->contains("4719") << endl;
    cout << lists->contains("4720") << endl;
    cout << lists->contains("4721") << endl;
    cout << lists->contains("4722") << endl;
    cout << lists->contains("4723") << endl;
    cout << lists->contains("4724") << endl;
    cout << lists->contains("4725") << endl;
    cout << lists->contains("4726") << endl;
    cout << lists->contains("4727") << endl;
    cout << lists->contains("4728") << endl;
    cout << lists->contains("4729") << endl;
    cout << lists->contains("4730") << endl;
    cout << lists->contains("4731") << endl;
    cout << lists->contains("4732") << endl;
    cout << lists->contains("4733") << endl;
    cout << lists->contains("4734") << endl;
    cout << lists->contains("4735") << endl;
    cout << lists->contains("4736") << endl;
    cout << lists->contains("4737") << endl;
    cout << lists->contains("4738") << endl;
    cout << lists->contains("4739") << endl;
    cout << lists->contains("4740") << endl;

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <uri>" << endl;
        cout << "       fetches the resource identified by <uri> and print it" << endl;
        return -1;
    }
}
