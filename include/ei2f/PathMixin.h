#ifndef DE_FRAUNHOFER_FKIE_EI2F_PATH_MIXIN_H
#define DE_FRAUNHOFER_FKIE_EI2F_PATH_MIXIN_H

#include <string>

#include <ei2f/common.h>
#include <ei2f/ObjectConfig.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::string;

/*! \brief Represents a convenience mixin for a path configuration
 *
 * Given a ObjectConfig this Mixin gives read access by the method path().
 */
class PathMixin {
    public:
        /*! \brief Initializes the PathMixin
         *
         * It the key does not exist, an InvalidConfiguration will be thrown.
         *
         * \param config The configuration to be applied. The constructor expects
         *               the key given by config_key to be present in the config.
         * \param config_key The key for the ObjectConfig instance where
                             the path should be read from.
         */
        PathMixin(const ObjectConfig& config, const char * config_key);
        virtual ~PathMixin();

        //! \brief Returns the path that was configured by the ObjectConfig
        const string& path() const { return _path; }

    private:
        string _path;
};

}}}} // Namespace de::fraunhofer::fkie::ei2f

#endif // DE_FRAUNHOFER_FKIE_EI2F_PATH_MIXIN_H

