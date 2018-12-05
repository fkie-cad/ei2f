#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BLOOMFILTER_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BLOOMFILTER_H

#include <unordered_set>
#include <string>
#include <istream>

#include <db.h>

#include <ei2f/plugin_common.h>
#include <ei2f/helper.h>
#include <ei2f/PathMixin.h>
#include <ei2f/backend/BinaryAPI.h>
#include <ei2f/backend/Backend.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;

/*! \brief Implements a bloom filter as a storage backend
 *
 * The following features are supported by the BloomFilter backend:
 *  - Persistent storage
 *  - Memory only storage
 *  - BinaryAPI based updaters
 *
 * \warning The complete filter data is placed into memory all time.
 */
class BloomFilter : public Backend, public PathMixin, public BinaryAPI {
    public:
        class DigestTooSmall : std::exception
        {
            const char * what () const throw ()
            {
                return "The configured Digest is too small for the supplied index!";
            }
        };

        // Implementation of Backend
        BloomFilter(const ObjectConfig& config);
        virtual ~BloomFilter();
        virtual ContainDecision contains(ItemInfo& item_info);
        virtual void flush(void);
        virtual void destroy(void);
        virtual BinaryAPI& binary_api(void);

        // Implementation of ItemAPI
        virtual void binary_ref(BLOB& blob);

    private:
        unsigned int get_index(const unsigned char* digest, unsigned int bitsize, unsigned int round);
        bool get_bit(unsigned int index);

        void load(void);

        char * _data; //!< Pointer to the bitarray data
        unsigned long _hash_bits; //!< The number of hash bits to consume per round
        unsigned long _hash_bit_offset; //!< The starting bit position of bits in the hash for first round
        unsigned long _data_bits;  //!< The number of bits of the filter bitarray
        unsigned long _data_bytes; //!< The number of bytes of the filter bitarray
        unsigned long _rounds; //!< The number of rounds the bloom filter will process

    // STATIC MEMBERS
    public:
        static Backend* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BLOOMFILTER_H
