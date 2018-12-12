#include <string>
#include <exception>

#include <ei2f/ItemInfo.h>
#include <ei2f/translator/Translator.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::out_of_range;
using translator::Translator;
using translator::TranslatorPtr;

const char * const cyclic_error_message = "A recursive call in ItemInfo Keys was detected. Usually this means that translators have a circular dependency.";

string& ItemInfo::get(const string& key)
{
    std::unordered_map<string,string>::iterator iter = _lookup.find(key);
    if (iter != _lookup.end()) {
        return iter->second;
    }

    // This will throw an exception if the key was already queried in get before in this
    // recursive call
    CycleGuard protector(_seen_db, key, cyclic_error_message);

    TranslatorPtr translator(Translator::create_from_key(key));

    if (!translator) {
        throw TranslatorNotFound();
    }
    translator->enrich(*this, key);
    return _lookup.at(key);
}

void
ItemInfo::set(const string& key, const string& value)
{
    _lookup[key] = value;
}

void
ItemInfo::reset(void)
{
    _lookup.clear();
}

}}}} // Namespace de::fraunhofer::fkie::ei2f
