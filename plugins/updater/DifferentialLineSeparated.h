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
