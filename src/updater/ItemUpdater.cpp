#include <ei2f/ObjectConfig.h>
#include <ei2f/updater/ItemUpdater.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

using namespace std;

ItemUpdater::ItemUpdater(const ObjectConfig& config) : Updater(config)
{
    // Needed parameters
    // None

    // Optional parameters
    config.extract_string(_item_key, "Updater.ItemKey", false, "Item");
}

ItemUpdater::~ItemUpdater()
{

}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater
