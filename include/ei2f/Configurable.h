#ifndef DE_FRAUNHOFER_FKIE_EI2F_CONFIGURABLE_H
#define DE_FRAUNHOFER_FKIE_EI2F_CONFIGURABLE_H

#include <ei2f/ObjectConfig.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

/*! \brief Provides a base class that stores a copy of an ObjectConfig.
 *
 * This class can be used as base class for all classes that need a permanent
 * copy of the ObjectConfig given to the object at construction time.
 */
class Configurable {
    public:
        /*!
         * Constructs a new Configurable instance configured by the passed config
         *
         * \param config A read only object config.
         */
        Configurable(const ObjectConfig& config) : _config(config) {}

        ~Configurable() {}

        /*! \brief Returns a reference to the object config
         *
         * This method returns a constant reference to the object embedded config
         * created as a copy of the config passed to the constructor. The lifetime of
         * the referenced ObjectConfig instance ends with the destruction of the
         * Configurable object
         */
        const ObjectConfig& config(void) const { return _config; }

    private:
        const ObjectConfig _config;
};

}}}} // Namespace de::fraunhofer::fkie::ei2f

#endif // DE_FRAUNHOFER_FKIE_EI2F_CONFIGURABLE_H
