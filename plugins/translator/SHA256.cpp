#include <Poco/DigestEngine.h>
#include <Poco/Crypto/DigestEngine.h>

#include <ei2f/ItemInfo.h>
#include <ei2f/translator/Translator.h>

#include "SHA256.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace translator {

using namespace std;
using Poco::Crypto::DigestEngine;

SHA256::SHA256() : Translator()
{
    // INTENTIONALLY LEFT EMPTY
}

SHA256::~SHA256()
{
    // INTENTIONALLY LEFT EMPTY
}

void SHA256::enrich_salted(ItemInfo& item_info, const string& key)
{
    DigestEngine eng("SHA256");
    bool full_calc = true;

    if (key == "SHA256.Salted.Binary") {
        if (item_info.available("SHA256.Salted.Hex")) {
            // Hex --> Binary
            string& hexdigest = item_info.get("SHA256.Salted.Hex");
            const DigestEngine::Digest& digest = eng.digestFromHex(hexdigest);
            string binary(digest.begin(), digest.end());
            item_info.set("SHA256.Salted.Binary", binary);
            full_calc = false;
        }

    } else if (key == "SHA256.Salted.Hex") {
        if (item_info.available("SHA256.Salted.Binary")) {
            // Binary --> Hex
            string& binary = item_info.get("SHA256.Salted.Binary");
            DigestEngine::Digest digest(binary.begin(), binary.end());
            item_info.set("SHA256.Salted.Hex", eng.digestToHex(digest));
            full_calc = false;
        }
    }

    if (full_calc) {
        // Item + Salt --> Binary + Hex
        string& item = item_info.get("Item");
        string& salt = item_info.get("Salt");
        eng.update(salt.c_str(), salt.size());
        eng.update(item.c_str(), item.size());
        const DigestEngine::Digest& digest = eng.digest();
        string binary(digest.begin(), digest.end());
        item_info.set("SHA256.Salted.Binary", binary);
        item_info.set("SHA256.Salted.Hex", eng.digestToHex(digest));
    }
}

void SHA256::enrich_unsalted(ItemInfo& item_info, const string& key)
{
    DigestEngine eng("SHA256");
    bool full_calc = true;

    if (key == "SHA256.Binary") {
        if (item_info.available("SHA256.Hex")) {
            // Hex --> Binary
            string& hexdigest = item_info.get("SHA256.Hex");
            const DigestEngine::Digest& digest = eng.digestFromHex(hexdigest);
            string binary(digest.begin(), digest.end());
            item_info.set("SHA256.Binary", binary);
            full_calc = false;
        }

    } else if (key == "SHA256.Hex") {
        if (item_info.available("SHA256.Binary")) {
            // Binary --> Hex
            string& binary = item_info.get("SHA256.Binary");
            DigestEngine::Digest digest(binary.begin(), binary.end());
            item_info.set("SHA256.Hex", eng.digestToHex(digest));
            full_calc = false;
        }
    }

    if (full_calc) {
        // Item --> Binary + Hey
        string& item = item_info.get("Item");
        eng.update(item.c_str(), item.size());
        const DigestEngine::Digest& digest = eng.digest();
        string binary(digest.begin(), digest.end());
        item_info.set("SHA256.Binary", binary);
        item_info.set("SHA256.Hex", eng.digestToHex(digest));
    }
}

void SHA256::enrich(ItemInfo& item_info, const string& key)
{
    if (key == "SHA256.Binary" || key == "SHA256.Hex") {
        enrich_unsalted(item_info, key);
    } else if (key == "SHA256.Salted.Binary" || key == "SHA256.Salted.Hex") {
        enrich_salted(item_info, key);
    }
}

Translator *
SHA256::create(void)
{
    return new SHA256();
}

void on_library_load(void) __attribute__((constructor));

void
on_library_load(void)
{
    cout << "de::fraunhofer::fkie::ei2f::translator::SHA256: Initializing Plugin" << endl;
    Translator::register_factory("SHA256.Binary", SHA256::create);
    Translator::register_factory("SHA256.Hex", SHA256::create);
    Translator::register_factory("SHA256.Salted.Binary", SHA256::create);
    Translator::register_factory("SHA256.Salted.Hex", SHA256::create);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::translator
