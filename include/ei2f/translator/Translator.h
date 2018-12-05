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
