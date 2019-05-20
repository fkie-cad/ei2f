/*
    Copyright (c) 2018-2019 Fraunhofer FKIE

    This file is part of EI2F.

    EI2F is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    EI2F is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with EI2F.  If not, see <http://www.gnu.org/licenses/>.
*/

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
