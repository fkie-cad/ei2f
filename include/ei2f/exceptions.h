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
