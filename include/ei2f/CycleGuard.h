#ifndef DE_FRAUNHOFER_FKIE_EI2F_CYCLE_GUARD_H
#define DE_FRAUNHOFER_FKIE_EI2F_CYCLE_GUARD_H

#include <string>
#include <unordered_set>
#include <exception>

#include <ei2f/common.h>
#include <ei2f/exceptions.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::string;

/*! \brief Represents a guard to protect against cyclic recursive calls based on a node key
 *
 * A key is defined by a string representing a node in a dependency/call graph
 * when a call for a specific key is made, the key will be added to the referenced SeenDB
 * when another CycleGuard is being created that refers to the same SeenDB detects the
 * same key appears another time, it will throw an exception. A cyclic dependency has
 * been detected.
 */
class CycleGuard {
    /****************
     * Nested Types *
     ****************/
    public:
        //! \brief The in memory data structure to hold the keys already seen
        typedef std::unordered_set<string> SeenDB;


    /********************
     * Instance Members *
     ********************/
    public:
        /*! \brief Creates a CycleGuard
         *
         * Usually this data structure should be created on the stack for the 
         * detection of cyclic call detection of a specific key that can be used for
         * matching. The method will throw an exception if the seen_db already contains
         * the key. Else it will add the key to the seen_db and it will remember the
         * seen_db and and the key until dtor runs.
         *
         * \param seen_db The in memory structure to represent the already traversed
         *                nodes.
         * \param key     The key representing the node that is currently entered.
         */
        CycleGuard(SeenDB& seen_db, const string& key, const char * error) : _seen_db(seen_db), _key(key)
        {
            if (seen_db.count(key) > 0) {
                throw exceptions::CycleDetected(error);
                // In this case dtor is NOT called for "this".
                // We didn't insert so we don't remove. It is the responsibility
                // of the other CycleGuard that set it first
            }
            seen_db.insert(key);
        }

        /*! \brief Stops the protection against cyclic recursive calls
         *
         * The destructor removes the associated key from the associated SeenDB.
         *
         * \warning The cyclic protection only lasts until the destruction of the guard.
         */
        ~CycleGuard(void) { _seen_db.erase(_key); }

    private:
        SeenDB& _seen_db;
        const string& _key;
};

}}}} // Namespace de::fraunhofer::fkie::ei2f

#endif // DE_FRAUNHOFER_FKIE_EI2F_CYCLE_GUARD_H
