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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BINARY_API_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BINARY_API_H

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace backend {

/*! \brief Represents the API for a backend with binary access

 * Implementations of Backends with updateable binary data structures must
 * implement this API. In contrast to the item based API this will allow updaters
 * to overwrite one defined BLOB area.
 */
class BinaryAPI {
    /****************
     * Nested Types *
     ****************/
    public:
        //! \brief Describes a reference to the updateable data structure
        struct BLOB {
            // TODO Check better size definition
            char * data; //!< Pointer to the updateable data structure
            unsigned int size; //!< Size of the updateable data structure
        };

    /********************
     * Instance Members *
     ********************/
    public:
        //! \brief Initializes the api object
        BinaryAPI() {}

        //! \brief Destroy the api object
        virtual ~BinaryAPI();

        /*! \brief Retrieves a reference to allow binary updates to the backend
         *
         * Backends supporting a binary update must implement this virtual method.
         */
        virtual void binary_ref(BLOB& blob) = 0;
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BINARY_API_H
