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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_TRANSLATOR_H
#define DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_TRANSLATOR_H

#include <string>
#include <unordered_map>
#include <memory>

#include <ei2f/helper.h>
#include <ei2f/ItemInfo.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

/*! \brief Contains the base and framework integrated implementations of a Translator
 *
 * A Translator represents a mechanism to translate values to specific item info key to
 * values other item info keys.
 */
namespace translator {

using std::string;

/*! \brief Represents code that may add key value pairs based on existing key value pairs.
 *
 * This class is the base class for all different types of Translators used
 * decorate the data represented by an instance of ItemInfo.
 *
 * \warning Translators are assumed to be not configurable nor
 *          should implementations have an instance state. Inheritance is only
 *          used for a flexible callback mechanism.
 */
class Translator {
    /****************
     * Nested Types *
     ****************/
    public:
        typedef Translator* (*FactoryFunctionPtr)(void);


    /********************
     * Instance Members *
     ********************/
    public:
        /*! \brief Initializes the base class instance of Translator. */
        Translator(void);

        /*! \brief Destroys the Translator. */
        virtual ~Translator(void);

        /*! \brief Tries to decorate the item_info with freshly a calculated value for key
         *
         * An implementation of this member must check whether it is possible to calculate
         * a value for the given key and the data in item_info. If successful the
         * key value pair must be added to the item_info. As a translator might be registered
         * for more than one key, this method is called with
         * the key as one of the parameters to be able to decide what to do.
         *
         * \param item_info The info object to be enriched
         * \param key       The key whose corresponding value should be calculated and
         *                  saved to the info object.
         */
        virtual void enrich(ItemInfo& item_info, const string& key) = 0;


    /******************
     * Static Members *
     ******************/
    public:
        /*! \brief Creates a Translator for a given key
         *
         * This is the factory method that generates an instance of an Translator
         * based on the given key that the translator should create
         *
         * \param key       The target key whose for that a translator is needed
         */
        static Translator * create_from_key(const string& key);

        /*! \brief Registers an implementation of a Translator
         *
         * A plugin implementing an Translator must call this method in it's library
         * in the initialization phase to register the factory function pointer
         * implementation. The same plugin might be registered to more than one key.
         *
         * \param key     The target key whose for that a translator is needed
         * \param factory The factory function pointer of the plugins Translator
         *                implementation.
         */
        static void register_factory(const string & key, const FactoryFunctionPtr factory);

    private:
        static std::unordered_map<std::string, FactoryFunctionPtr> _factory_map;
};

typedef std::shared_ptr<Translator> TranslatorPtr;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::translator

#endif // DE_FRAUNHOFER_FKIE_EI2F_TRANSLATOR_FRANSLATOR_H
