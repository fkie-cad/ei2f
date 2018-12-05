#include <vector>
#include <iostream>
#include <string>
#include <cstring>

#include <ei2f/common.h>
#include <ei2f/patterns.h>
#include <ei2f/exceptions.h>
#include <ei2f/transport/Transport.h>

#include "Online.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using transport::Transport;
using transport::TransportPtr;
using namespace std;

Online::Online(const ObjectConfig& config) : Backend(config)
{
    // Mandatory parameters
    config.extract_string(_uri_pattern, "Backend.UriPattern", true, "");
    config.extract_string(_compare_item_key, "Backend.CompareItemKey", true, "");

    // Optional Parameters
    config.extract_unsigned_long(_key_offset, "Backend.KeyOffset", false, 0, 0, 0xffffffff);
    config.extract_unsigned_long(_key_length, "Backend.KeyLength", false, 0xffffffff, 0, 0xffffffff);

    // Other
    _need_key_cut = (_key_length != 0xffffffff) || (_key_offset != 0);

    if (ends_with(item_key(), ".Binary")) {
        throw exceptions::InvalidConfiguration("Backend.ItemKey is selected with Binary type, this is not allowed!");
    }
}

Online::~Online()
{
    // INTENTIONELLY LEFT BLANK
}

ContainDecision
Online::contains(ItemInfo& item_info)
{
    string key = item_info.get(item_key());
    string uri = uri_pattern();

    if (_need_key_cut) {
        key = key.substr(_key_offset, _key_length);
    }

    replace_all(uri, patterns::key, key);

    TransportPtr transport(Transport::create_from_uri(uri, config()));
    istream& input = transport->uri_to_istream(uri);

    // get salted_hash
    string salt;
    getline(input, salt);
    item_info.set("Salt", salt);
    string salted_hash = item_info.get(compare_item_key());

    // get server side salted_hash_candidates
    std::unordered_set<string> salted_hash_candidates;
    string salted_hash_candidate;

    while (getline(input, salted_hash_candidate)) {
        salted_hash_candidates.insert(salted_hash_candidate);
    }

    // check if candidates contain or local candidate
    if (salted_hash_candidates.find(salted_hash) != salted_hash_candidates.end()) {
        // match found
        return ContainDecision::contained;
    } else {
        return ContainDecision::not_contained;
    }
}

Backend *
Online::create_from_config(const ObjectConfig& config)
{
    return new Online(config);
}

void on_library_load(void) __attribute__((constructor));

void
on_library_load(void)
{
    cout << "de::fraunhofer::fkie::ei2f::backend::Online: Initializing Plugin" << endl;
    Backend::register_factory("Online", Online::create_from_config);
    Backend::register_factory("de::fraunhofer::fkie::ei2f::backend::Online", Online::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend
