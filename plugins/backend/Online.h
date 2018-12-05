#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_ONLINE_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_ONLINE_H

#include <ei2f/plugin_common.h>

#include <unordered_set>
#include <string>
#include <db.h>
#include <istream>

#include <ei2f/helper.h>
#include <ei2f/backend/Backend.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;

/*! \brief Implements a remote lookup only Backend
 *
 * This implementation takes the item value defined by "Backend.ItemKey"
 * and uses it for a lookup query against Backend.UriPattern.
 * before using it in placeholder {Key} it will be optionally trimmed
 * by the parameters Backend.KeyOffset and Backend.KeyLength (both in
 * characters).
 *
 * The resulting answer will be interpreted in the following way:
 *
 * The first line will be used as a salt for further calculations.
 * The remaining lines are interpreted as salted hash of the item
 * candidates. Thus after calculating the salted hash of our item,
 * it is checked whether the result corresponds to one of the received
 * lines. If so, the item is to be considered as "contained" in the backend
 * else not.
 *
 * The following keys are used to configure this Backend in addition
 * to inherited configuration items
 *
 * Configuration key      | Description
 * -----------------------|--------------------------------
 * Backend.UriPattern     | Uri to query for the item_key
 * Backend.CompareItemKey | Key to compare results of the query
 * Backend.KeyOffset      | Character offset to start for calculation of lookup key
 * Backend.KeyLength      | character length for calculation of lookup key
 *
 * No updateable APIS are supported by this backend.
 * There is no local storage used.
 */
class Online : public Backend {
    public:
        class DigestTooSmall : std::exception
        {
            const char * what () const throw ()
            {
                return "The configured Digest is too small for the supplied index!";
            }
        };

        // Backend implementation
        Online(const ObjectConfig& config);
        virtual ~Online();
        virtual ContainDecision contains(ItemInfo& item_info);

        const string& compare_item_key(void) { return _compare_item_key; }
        unsigned long key_offset(void) { return _key_offset; }
        unsigned long key_length(void) { return _key_length; }
        const string& uri_pattern(void) const { return _uri_pattern; }

    private:
        unsigned long _key_offset;
        unsigned long _key_length;
        bool _need_key_cut;
        string _uri_pattern;
        string _compare_item_key;

    // STATIC MEMBERS
    public:
        static Backend* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_ONLINE_H
