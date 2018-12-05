#include <ei2f/backend/ItemAPI.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

using namespace std;

// class Backend::ReadIterator
ItemAPI::ReadIterator::~ReadIterator()
{
    // INTENTIONALLY LEFT EMPTY
}

void
ItemAPI::add(vector<ItemInfo>& item_infos)
{
    for (auto item_info: item_infos)
        add(item_info);
}

void
ItemAPI::remove(vector<ItemInfo>& item_infos)
{
    for (auto item_info: item_infos)
        remove(item_info);
}

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend
