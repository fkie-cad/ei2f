#ifndef DE_FRAUNHOFER_FKIE_EI2F_EXCEPTIONS_H
#define DE_FRAUNHOFER_FKIE_EI2F_EXCEPTIONS_H

#include <exception>
#include <string>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace exceptions {

using std::string;

/*! \brief Represents a configuration error in a ObjectConfig given to a Configurable
 *
 * Parameters given by an ObjectConfig might be missing and manadatory or
 * invalid values. In this case, an InvalidConfiguration Exception is thrown
 */
class InvalidConfiguration : public std::invalid_argument {
    public:
        explicit InvalidConfiguration(const string& what_arg) : std::invalid_argument(what_arg) {}
        explicit InvalidConfiguration(const char* what_arg) : std::invalid_argument(what_arg) {}
};

/*
class MissingMandatoryParamater : public InvalidConfiguation {
    public:
        explicit MissingMandatoryParameter(const char* parameter);
        explicit MissingMandatoryParameter(const string& parameter);
};
*/

/*! \brief Represents an runtime update error
 *
 * When an update fails an UpdateFailed Exception is thrown.
 */
class UpdateFailed : public std::runtime_error {
    public:
        explicit UpdateFailed(const string& what_arg) : std::runtime_error(what_arg) {}
        explicit UpdateFailed(const char* what_arg) : std::runtime_error(what_arg) {}
};

/*! \brief Represents an exception for calls to unsupported APIs
 *
 * When users of objects call to APIs not supported by the implementation
 * an APINotSupported exception is thrown.
 */
class APINotSupported : public std::runtime_error {
    public:
        explicit APINotSupported(const string& what_arg) : std::runtime_error(what_arg) {}
        explicit APINotSupported(const char* what_arg) : std::runtime_error(what_arg) {}
};

/*! \brief Represents an exception for a runtime detected recurcive cyclic call dependency
 *
 * Currently when separate Translators are used and they define a cyclic dependency of
 * item info keys, at calculation (runtime) time an CycleDetected Exception will be thrown
 */
class CycleDetected : public std::runtime_error {
    public:
        explicit CycleDetected(const string& what_arg) : std::runtime_error(what_arg) {}
        explicit CycleDetected(const char* what_arg) : std::runtime_error(what_arg) {}
};

}}}}} // namespace de::fraunhofer::fkie::ei2f::exceptions

#endif // DE_FRAUNHOFER_FKIE_EI2F_EXCEPTIONS_H
