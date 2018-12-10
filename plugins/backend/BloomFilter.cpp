#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>

//#include <ei2f/helper.h>
#include "BloomFilter.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;

BloomFilter::BloomFilter(const ObjectConfig& config) : Backend(config), PathMixin(config, "Backend.Path"), _data(NULL), _hash_bits(0), _data_bits(0), _data_bytes(0)
{
    // Mandatory parameters
    config.extract_unsigned_long(_hash_bits, "Backend.HashBits", true, 0, 0, 32);

    // Optional Parameters
    config.extract_unsigned_long(_rounds, "Backend.Rounds", false, 1, 1, 256);
    config.extract_unsigned_long(_hash_bit_offset, "Backend.HashBitsOffset", false, 0, 0, 0xffffffff);

    // Other stuff
    if (!ends_with(item_key(), ".Binary")) {
        cerr << "de::fraunhofer::fkie::ei2f::backend::BloomFilter: Backend.ItemKey is not selected optimally, you should use a Binary type." << endl;
    }

    _data_bits = 1 << _hash_bits;
    _data_bytes = (_data_bits + 7) >> 3;
    _data = (char*) calloc(_data_bytes, 1);

    try {
        load();
    } catch (...) {
        free(_data);
        throw;
    }
}

BloomFilter::~BloomFilter()
{
    if (_data) {
        free(_data);
        _data = NULL;
    }
}

unsigned int
BloomFilter::get_index(const unsigned char* digest, unsigned int bitsize, unsigned int round)
{
    unsigned int begin_bitpos = round * _hash_bits + _hash_bit_offset;
    unsigned int end_bitpos = begin_bitpos + _hash_bits - 1;

    if ((begin_bitpos >= bitsize) || (end_bitpos >= bitsize)) {
        throw DigestTooSmall();
    }

    unsigned int bytepos = begin_bitpos >> 3;
    unsigned int bitpos = begin_bitpos - (bytepos << 3);

    unsigned int end_bytepos = end_bitpos >> 3;
    end_bitpos = end_bitpos - (end_bytepos << 3);

    unsigned long long result = 0;
    unsigned long long insert_offset = 0;
    unsigned long long insert_length = 8 - bitpos;
    unsigned long long snippet;

    while (bytepos < end_bytepos) {
        snippet = digest[bytepos];
        snippet >>= bitpos;
        snippet &= (1l << insert_length) - 1;
        result |= snippet << insert_offset;

        // Reset for next rount
        insert_offset += insert_length;
        insert_length = 8;
        bitpos = 0;
        bytepos++;
    }

    // last byte
    insert_length -= 7 - end_bitpos;

    snippet = digest[bytepos];
    snippet >>= bitpos;
    snippet &= (1l << insert_length) - 1;
    result |= snippet << insert_offset;

    return result;
}

bool
BloomFilter::get_bit(unsigned int bitindex)
{
    unsigned int byteindex = bitindex >> 3;
    bitindex = bitindex - (byteindex << 3);
    return (_data[byteindex] & (1<<bitindex)) != 0;
}

ContainDecision
BloomFilter::contains(ItemInfo& item_info)
{
    string binary_digest_str = item_info.get(item_key());
    const unsigned char * digest = (const unsigned char *) binary_digest_str.c_str();
    const unsigned int bitsize = binary_digest_str.size() * 8;

    unsigned int index;

    for (unsigned int round = 0; round < _rounds; ++round) {
        index = get_index(digest, bitsize, round);
        if (!get_bit(index)) return not_contained;
    }
    return undecided;
}

void
BloomFilter::load(void)
{
    const string& p = path();
    if (p != "") {
        ifstream f(p);
        f.read(_data, _data_bytes);
    }
}

Backend *
BloomFilter::create_from_config(const ObjectConfig& config) {
    return new BloomFilter(config);
}

BinaryAPI&
BloomFilter::binary_api(void)
{
    return *this;
}


/****************************************************
 * class BloomFilter (part BinaryAPI)               *
 ****************************************************/

void
BloomFilter::binary_ref(BLOB& blob)
{
    blob.data = _data;
    blob.size = _data_bytes;
}

void
BloomFilter::flush(void)
{
    const string& p = path();
    if (p != "") {
        ofstream of(p);
        of.write(_data, _data_bytes);
    }
}

void
BloomFilter::destroy(void)
{
    const string& p = path();
    if (p != "") {
        remove(p.c_str());
    }
}

extern "C" {
    extern void init_ei2f_plugin(void);
};

void
init_ei2f_plugin(void)
{
    cout << "de::fraunhofer::fkie::ei2f::backend::BloomFilter: Initializing Plugin" << endl;
    Backend::register_factory("BloomFilter", BloomFilter::create_from_config);
    Backend::register_factory("de::fraunhofer::fkie::ei2f::backend::BloomFilter", BloomFilter::create_from_config);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend
