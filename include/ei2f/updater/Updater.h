#ifndef DE_FRAUNHOFER_FKIE_EI2F_UPDATER_UPDATER_H
#define DE_FRAUNHOFER_FKIE_EI2F_UPDATER_UPDATER_H

#include <string>
#include <memory>

#include <ei2f/Configurable.h>
#include <ei2f/ObjectConfig.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

class Stage;

/*! \brief Contains the base and framework integrated implementations of an Updater
 *
 * An Updater represent a way of updating Backends with new data by supported APIs.
 */
namespace updater {

using std::string;

/*! \brief Represents a Updater for one stage of the list pipeline.
 *
 * An Updater is responsible for updateing the backends.
 * An Updater always updates one stage at a time.
 * Curently updaters are configured and instantiated by Stage.
 */
class Updater : public Configurable {
    /****************
     * Nested Types *
     ****************/
    public:
        typedef Updater* (*FactoryFunctionPtr) (const ObjectConfig& confi);


    /********************
     * Instance Members *
     ********************/
    public:
        /*! \brief Initializes the Updater base class object based on the given object config.
         *
         * \param config The configuration to be applied.
         *               The constructor expects the following config keys:
         *               - Updater.UpdateUri (mandatory)
         *               - Updater.VersionUriPattern (optional)
         *               - Updater.InPlace (optional)
         */
        Updater(const ObjectConfig& dl);

        //! \brief No assignment of Updater objects
        Updater& operator=(const Updater&) = delete;

        //! \brief No copies of Updater objects
        Updater(const Updater&) = delete;

        /*! \brief Destroy the object
         *
         * This base class destructor does nothing.
         */
        virtual ~Updater();

        /*! \brief Provides new version information
         *
         * This method provides a way to retrieve the info whether there is a newer
         * version available based on the current version and additionally what the
         * new version's versions string is.
         *
         * The default implementation is either:
         *  - when no VersionUriPattern supplied: set the new_version to current version
         *    and always signal that an update is available
         *  - when VersionUriPattern supplied: request the content and interpret
         *    the first line as version string. It returns true, when the new version string
         *    not equals the old version string
         *
         * Specific updates might overwrite this logic when needed.
         */
        virtual bool update_available(const string& current_version, string& new_version);

        /*! \brief Updates the given stage to the given version
         *
         * An implementation of the Updater must implement this method.
         * It can access the stage to create temporary or new backends.
         * At the end it must promote the resulting backend in the stage and
         * destroy all temporary backends.
         *
         * \param stage       The stage object whose backend is to be updated
         * \param new_version The target version string that should be updated to
         */
        virtual void update_to(Stage& stage, const string& new_version) = 0;

        /*! \brief Returns whether the Update should be done in place or not
         *
         * If implementations have the possibility to either do an inplace update
         * or create a new Backend and promote the new Backend in the Stage object,
         * the implementation must obey the return value
         *
         * \return Boolean whether the implementation must do an in place update or
         *         a separated new backend update
         */
        bool in_place(void) const { return _in_place; }

        //! \brief Returns the configured uri pattern where the update can be downloaded
        const string& uri_pattern(void) const { return _uri_pattern; }

        //! \brief Returns the configured uri pattern where the version info can be downloaded
        const string& version_uri_pattern(void) const { return _version_uri_pattern; }

    protected:
        /*! \brief Replacees the given uri pattern with predefined values
         *
         * \warning The current behaviour is not yet completely specified
         *          TODO: Specify
         */
        void pattern_to_uri(string& uri, Stage& stage, const string& new_version);

    private:
        string _uri_pattern;
        string _version_uri_pattern;
        bool   _in_place;

    /******************
     * Static Members *
     ******************/
    public:
        static void register_factory(const string& updater, const FactoryFunctionPtr factory);
        static Updater* create_from_config(const ObjectConfig& config);

    private:
        static std::unordered_map<string, FactoryFunctionPtr> _factory_map;
};

typedef std::shared_ptr<Updater> UpdaterPtr;

}}}}} // Namespace de::fraunhofer::fkie::ei2f::Updater

#endif // DE_FRAUNHOFER_FKIE_EI2F_UPDATER_UPDATER_H
