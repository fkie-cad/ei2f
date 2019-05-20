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

#ifndef DE_FRAUNHOFER_FKIE_EI2F_UPDATER_DIFFERENTIAL_LINE_SEPARATED_H
#define DE_FRAUNHOFER_FKIE_EI2F_UPDATER_DIFFERENTIAL_LINE_SEPARATED_H

#include <ei2f/plugin_common.h>

#include <string>

#include "Line.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

class Stage;

namespace updater {

class DifferentialLineSeparated: public Line {
    // INSTANCE MEMBERS
    public:
        DifferentialLineSeparated(const ObjectConfig& config);
        ~DifferentialLineSeparated();

        virtual void update_to(Stage& stage, const string& update_version);

        const string& add_uri_pattern(void) const { return _add_uri_pattern; }
        const string& remove_uri_pattern(void) const { return _remove_uri_pattern; }
        const string& version_uri_pattern(void) const { return _version_uri_pattern; }

    protected:
        void remove_lines_from_backend(istream& input, ItemAPI& item_api);
        void new_line_event(const string & line, Backend& target_backend, Backend& remove_backend);
        void noninplace_update_to(Stage& stage, const string& update_version);
        void inplace_update_to(Stage& stage, const string& update_version);

        string _add_uri_pattern;
        string _remove_uri_pattern;
        string _version_uri_pattern;

    // STATIC MEMBERS
    public:
        static Updater* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater

#endif // DE_FRAUNHOFER_FKIE_EI2F_UPDATER_DIFFERENTIAL_LINE_SEPARATED_H
