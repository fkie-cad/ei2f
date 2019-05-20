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

#include <Poco/DigestEngine.h>
#include <Poco/Crypto/DigestEngine.h>

#include <ei2f/ItemInfo.h>
#include <ei2f/translator/Translator.h>

#include <openssl/sha.h>

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

static const string pattern_sha256_binary("SHA256.Binary");
static const string pattern_item("Item");

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
    if (key == pattern_sha256_binary) {
        string& value = item_info.get(pattern_item);
        unsigned char hash[SHA256_DIGEST_LENGTH];

        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, value.c_str(), value.size());
        SHA256_Final(hash, &sha256);

        string data((const char *)&hash, SHA256_DIGEST_LENGTH);
        item_info.set(pattern_sha256_binary, data);

    } else if (key == "SHA256.Binary" || key == "SHA256.Hex") {
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

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::translator::SHA256: Initializing Plugin" << endl;
    Translator::register_factory("SHA256.Binary", SHA256::create);
    Translator::register_factory("SHA256.Hex", SHA256::create);
    Translator::register_factory("SHA256.Salted.Binary", SHA256::create);
    Translator::register_factory("SHA256.Salted.Hex", SHA256::create);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::translator
