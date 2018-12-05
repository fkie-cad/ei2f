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
