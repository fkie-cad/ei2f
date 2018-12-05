#ifndef DE_FRAUNHOFER_FKIE_EI2F_UPDATER_DIFFERENTIAL_LINE_H
#define DE_FRAUNHOFER_FKIE_EI2F_UPDATER_DIFFERENTIAL_LINE_H

#include <string>

#include <ei2f/plugin_common.h>
#include <ei2f/backend/Backend.h>

#include "Line.h"

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {
namespace updater {

class DifferentialLine: public Line {
    // INSTANCE MEMBERS
    public:
        DifferentialLine(const ObjectConfig& config);
        ~DifferentialLine();

        virtual void update_to(Stage& stage, const string& new_version);

    protected:
        void new_line_event(const std::string & line, ItemAPI& target_backend, ItemAPI& remove_backend);
        void key_value_event(const std::string & key, const std::string & value);
        void inplace_update_to(Stage& stage, const string& new_version);
        void noninplace_update_to(Stage& stage, const string& new_version);

    // STATIC MEMBERS
    public:
        static Updater* create_from_config(const ObjectConfig& config);
};

}}}}} // Namespace de::fraunhofer::fkie::ei2f::updater

#endif // DE_FRAUNHOFER_FKIE_EI2F_UPDATER_DIFFERENTIAL_LINE_H
