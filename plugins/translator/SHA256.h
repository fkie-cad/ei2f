#ifndef DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_SHA256_H
#define DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_SHA256_H

#include <string>

#include <ei2f/common.h>
#include <ei2f/ItemInfo.h>
#include <ei2f/translator/Translator.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace translator {

using std::string;

/*! \brief Implements for SHA256 based item info keys
 *
 * This translator implements the following item key conversions
 *
 * Calculated Item      | based on             | Comment
 * ---------------------|----------------------|------------------
 * SHA256.Binary        | SHA256.Hex           | Simple conversion
 * SHA256.Binary        | Item                 | Calculation
 * SHA256.Hex           | SHA256.Binary        | Simple conversion
 * SHA256.Hex           | Item                 | Calculation
 * SHA256.Salted.Binary | SHA256.Salted.Hex    | Simple converison
 * SHA256.Salted.Binary | Item, Salt           | Calculation
 * SHA256.Salted.Hex    | SHA256.Salted.Binary | Simple conversion
 * SHA256.Salted.Hex    | Item, Salt           | Calculation
 */
class SHA256 : public Translator {
    // INSTANCE MEMBERS
    public:
        SHA256();
        virtual ~SHA256();

        virtual void enrich(ItemInfo& item_info, const string& key);
    private:
        void enrich_salted(ItemInfo& item_info, const string& key);
        void enrich_unsalted(ItemInfo& item_info, const string& key);

    // STATIC MEMBERS
    public:
        static Translator * create();

};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::translator

#endif // DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_SHA256_H
