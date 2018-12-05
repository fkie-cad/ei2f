#ifndef DE_FRAUNHOFER_FKIE_EI2F_UPDATER_LINE_H
#define DE_FRAUNHOFER_FKIE_EI2F_UPDATER_LINE_H

#include <fstream>
#include <string>

#include <ei2f/plugin_common.h>
#include <ei2f/updater/ItemUpdater.h>
#include <ei2f/backend/Backend.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using std::string;
using std::istream;
using backend::ItemAPI;

class Line : public ItemUpdater {
    // INSTANCE MEMBERS
    public:
        Line(const ObjectConfig& config);
        virtual ~Line();

        virtual void update_to(Stage& stage, const string& new_version);

    protected:
        void add_lines_to_backend(istream& input, ItemAPI& item_api);

    // STATIC MEMBERS
    public:
        static Updater* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater

#endif // DE_FRAUNHOFER_FKIE_EI2F_UPDATER_LINE_H
