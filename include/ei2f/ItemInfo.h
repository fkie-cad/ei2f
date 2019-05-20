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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_ITEMINFO_H
#define DE_FRAUNHOFER_FKIE_EI2F_ITEMINFO_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <exception>
#include <iostream>

#include <ei2f/common.h>
#include <ei2f/CycleGuard.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::string;

/*! \brief Describes an item to be searched when querying a list pipeline
 *
 * This class represents a string only based dictionary. Both keys and values
 * currently can only be of the type std::string.
 *
 * \warning Keep in mind that the value's strings might transport binary data. This should
 *          be indicated by the key's suffix ".Binary".
 *
 * In addition to the lookup storage, this class implements the logic to
 * automatically trigger a translator query when an item key is not present yet.
 * Therefor it queries the registered Translator plugin for the queried key
 * to create the missing value.
 */
class ItemInfo {
    /****************
     * Nested Types *
     ****************/
    public:
        class TranslatorNotFound : public std::exception {
            const char * what () const throw ()
            {
                return "The requested key could neither be found nor calculated! Maybe the ItemKey configs are wrong or a translator plugin is missing.";
            }
        };


    /********************
     * Instance Members *
     ********************/
    public:
        /*! \brief Constructs an empty ItemInfo instance.
         *
         * This constructor can be used to create an empty ItemInfo instance.
         */
        ItemInfo() {}

        /*! \brief Constructs an ItemInfo instance filled with one key value pair.
         *
         * This constructor is used as a convenience alternative to create and fill
         * an instance of ItemInfo in one single step.
         * 
         * \param key The key to be used for the first element pair
         * \param value The value to be used for the first element pair
         */
        ItemInfo(const string& key, const string& value) { _lookup[key] = value; }

        ~ItemInfo() {}

        /*! \brief Retrieves the value for the given key
         *
         * This method retrieves the value for the given key.
         * If not present in the internal lookup table it calls into one of the registered
         * translator plugins to calculate the value for the requested key and stores
         * the result into the internal lookup table
         *
         * \param key The key to be queried
         *
         * \return the value that corresponds to the given key.
         */
        string& get(const string& key);

        /*! \brief Manually sets the value for a given key
         *
         * This method manually sets a key value pair in the internal lookup table
         *
         * \param key The key for the pair to be stored
         * \param value The value for the pair to be stored
         */
        void set(const string& key, const string& value);

        /*! \brief Resets the instance to an empty key value lookup table.
         *
         * This method deletes all entries from the internal lookup table.
        */
        void reset(void);

        /*! \brief Resets the instance to contain exactly one key value pair.
         *
         * This method deletes all entries from the internal lookup table.
         * After that it adds one pair given by the parameters of the call.
         *
         * \param key The key of the pair to be added
         * \param value The value of the pair to be added
         */
        void reset(const string& key, const string& value) { reset(); _lookup[key] = value; }

        /*! \brief Checks whether the key is in the already present entries
         *
         * Checks whether the key is in the already present entries. This will not
         * trigger any translators to be used.
         *
         * \param key The key to be checked
         *
         * \return true if the key exists, else false
         */
        bool available(const string& key) {
            return _lookup.find(key) != _lookup.end();
        }

    private:
        std::unordered_map<string,string> _lookup;
        CycleGuard::SeenDB _seen_db;
};

}}}} // Namespace de::fraunhofer::fkie::ei2f

#endif // DE_FRAUNHOFER_FKIE_EI2F_ITEMINFO_H
