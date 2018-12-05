#ifndef DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_FILE_H
#define DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_FILE_H

#include <fstream>

#include <ei2f/plugin_common.h>
#include <ei2f/transport/Transport.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace transport {

using std::istream;
using std::ifstream;
using std::string;

/*! \brief Represents transporting data from a file.
 * TODO FIX COPY PASTE
 * This class is the base class for all different types of Transports used
 * to load local or remote (update) resources
 */
class File : public Transport {
    // INSTANCE MEMBERS
    public:
        File(const ObjectConfig& config);
        virtual ~File();

        virtual istream& uri_to_istream(const string& raw_uri);

    private:
        ifstream _file;

    // STATIC MEMBERS
    public:
        static Transport * create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::transport

#endif // DE_FRAUNHOFER_FKIE_EI2F_TRANSPORT_FILE_H
