#ifndef DOMAINLIST_COMMON_H
#define DOMAINLIST_COMMON_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::string;

enum ContainDecision { not_contained=0, undecided=1, contained=2 };

//typedef std::unordered_map<string,string> ItemInfo;

class Stage;

/*
typedef std::shared_ptr<Stage> StagePtr;
typedef std::shared_ptr<Transport> TransportPtr;
typedef std::shared_ptr<backend::Backend> BackendPtr;
typedef std::shared_ptr<updater::Updater> UpdaterPtr;
*/
}}}}


#endif // DOMAINLIST_COMMON_H
