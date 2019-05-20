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

