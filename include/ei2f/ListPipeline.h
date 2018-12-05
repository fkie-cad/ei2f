#ifndef DE_FRAUNHOFER_FKIE_EI2F_LIST_PIPELINE_H
#define DE_FRAUNHOFER_FKIE_EI2F_LIST_PIPELINE_H

#include <list>

#include <ei2f/Stage.h>

namespace de {
namespace fraunhofer {
namespace fkie {
namespace ei2f {

using std::string;

class ListPipeline {
    // INSTANCE MEMBERS
    public:
        ListPipeline(const string& config_path);
        bool contains(const string& item);

    protected:
        void add_stage(StagePtr domain_list);
        void clear_stages(void);

    private:
        std::list<StagePtr> _stages;
};

}}}} // Namespace de::fraunhofer::fkie:ei2f

#endif // DE_FRAUNHOFER_FKIE_EI2F_LIST_PIPELINE_H
