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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BACKEND_H
#define DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BACKEND_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <exception>

#include <ei2f/common.h>
#include <ei2f/helper.h>
#include <ei2f/Configurable.h>
#include <ei2f/ItemInfo.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

/*! \brief Contains the base and framework integrated implementations of a Backend
 *
 * Backends represent a way of storing or looking up whether a given item belongs to
 * a specific list or not.
 */
namespace backend {

class ItemAPI;
class BinaryAPI;

using std::string;

/*! \brief Represents a storage backend of one stage of the list pipeline.
 *
 * This class is the base class for all different types of backends used
 * to store or query the membership information of a given item.
 *
 * Implementations must consider the following
 * -------------------------------------------
 * When writing a backend implementation, if the backend to be implemented might
 * store data locally, you should consider adding the PathMixin plugin
 * to your implementation class.
 *
 * Backends that should be updated must at least implement one ItemAPI or BinaryAPI
 * and return a handle to the api by Backend::item_api or Backend::binary_api.
 *
 * Backends that support persistence must overwrite Backend::flush to persist not
 * yet persisted data. Currently a Backend does not need to implement any kind of transactional
 * behaviour. It may or may not only persist data on the call to flush,
 * but also already when adding or removing data. This is implementation specific.
 *
 * Backend implementations (especially those that support persistence)
 * must destroy all ressources associated with the backend in a call to Backend::destroy.
 * In contrast to the destructor, this method permanently also destroys persistent
 * data.
 *
 * Backend implementations may not delete any persisted data in a normal run of the
 * the destructor. This is to be done in Backend::destroy.
 */
class Backend : public Configurable {
    /********************
     * Instance Members *
     ********************/
    public:
        /*! \brief Initializes the backend base class object based on the given object
         *         config.
         *
         * \param config The configuration to be applied. The constructor expects the keys
         *               "Backend.UUID" and "Backend.ItemKey" to be present in the config.
         */
        Backend(const ObjectConfig& config);

        Backend& operator=(const Backend&) = delete;
        Backend(const Backend&) = delete;

        /*! \brief Destruct the object
         *
         * The destructor does not and implementations must not delete persisted data in
         * their destructor. To delete persisted data, use method destroy.
         */
        virtual ~Backend();

        /*! \brief Returns the uuid of the backend
         *
         *  \return The lifetime of the string object ends with the destruction of the
         *          Backend.
         */
        const string& uuid(void) const { return _uuid; }

        /*! \brief Retrieves the configured ItemKey
         *
         * \return The configured ItemKey. The returned value's object's lifetime ends with
         *         the destruction of the Backend on which the method has been called.
         */
        const string& item_key(void) const { return _item_key; }

        // TODO COMMENT
        virtual ItemAPI& item_api(void);
        virtual BinaryAPI& binary_api(void);

        /*! \brief Flushes the backends data to persistent storage if needed.
         *
         * If persistence is enable, this method flushes all outstanding
         * changes to disk. If persistence is disabled, this method
         * does nothing.
         */
        virtual void flush(void);

        /*! \brief Destroys the backend
         *
         * A call to this method will release all resources associated with the Backend.
         * It will delete all files on disk, when persistence is enabled.
         *
         * \warning It is unspecified what happens when using a backend after calling destroy.
         */
        virtual void destroy(void);

        /*! \brief Checks whether the specified item is in the database
         *
         * This method checks whether the specified item can be found in the backend.
         */
        virtual ContainDecision contains(ItemInfo& info) = 0;

    private:
        string _item_key;
        string _uuid;


    /******************
     * Static Members *
     ******************/
    public:
        /*! \brief Defines the type of FactoryFunctionPtr for the Backend factory
         */
        typedef Backend* (*FactoryFunctionPtr)(const ObjectConfig& config);

        /*! \brief Creates a Backend for a given ObjectConfig
         *
         * This is the factory method to create Backends
         * (called from de::fraunhofer::fkie::ei2f::Stage).
         *
         * \param config The configuration to be used for instantiating the Backend.
                         It must include a value for the key "Backend.Type".
         */
        static Backend* create_from_config(const ObjectConfig& config);

        /*! \brief Registers an implementation of a Backend
         *
         * A plugin implementing a backend must call this method in it's library
         * in the initialization phase to register the factory function pointer
         * implementations.
         *
         * \param type The name of the type to be used for registering this backend
         * \param factory The factory function pointer of the plugins Backend
         * implementation.
         */
        static void register_factory(const string & type, const FactoryFunctionPtr factory);

    private:
        static std::unordered_map<std::string, FactoryFunctionPtr> _factory_map;
};

typedef std::shared_ptr<Backend> BackendPtr;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::backend

#endif // DE_FRAUNHOFER_FKIE_EI2F_BACKEND_BACKEND_H
