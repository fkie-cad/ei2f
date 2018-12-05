#ifndef DE_FRAUNHOFER_FKIE_EI2F_UPDATER_BINARY_H
#define DE_FRAUNHOFER_FKIE_EI2F_UPDATER_BINARY_H

#include <string>
#include <fstream>

#include <ei2f/plugin_common.h>
#include <ei2f/updater/Updater.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using de::fraunhofer::fkie::ei2f::backend::BinaryAPI;
using std::istream;

class Binary : public Updater {
    // INSTANCE MEMBERS
    public:
        Binary(const ObjectConfig& config);
        virtual ~Binary();

        virtual void update_to(Stage& stage, const string& new_version);

    protected:
        void stream_to_blob(istream& input, BinaryAPI::BLOB& blob);

    // STATIC MEMBERS
    public:
        static Updater* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater

#endif // DE_FRAUNHOFER_FKIE_EI2F_UPDATER_BINARY_H
