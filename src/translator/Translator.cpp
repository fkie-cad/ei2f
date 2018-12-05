#include <iostream>

#include <ei2f/common.h>
#include <ei2f/exceptions.h>
#include <ei2f/translator/Translator.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace translator {

using namespace std;

Translator::Translator()
{
    // INTENTIONALLY LEFT EMPTY
}

Translator::~Translator()
{
    // INTENTIONALLY LEFT EMPTY
}

Translator *
Translator::create_from_key(const string& key)
{
    FactoryFunctionPtr create;
    try {
        create = _factory_map.at(key);
    } catch (out_of_range& e) {
        string msg = "Unknown key '";
        msg += key;
        msg += "' for ItemInfoTranslator. Maybe a corresponding plugin is missing.";
        throw exceptions::InvalidConfiguration(msg);
    }
    return create();
}

void
Translator::register_factory(const string& key, const Translator::FactoryFunctionPtr factory)
{
    _factory_map[key] = factory;
}

unordered_map<string, Translator::FactoryFunctionPtr> Translator::_factory_map;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::translator
