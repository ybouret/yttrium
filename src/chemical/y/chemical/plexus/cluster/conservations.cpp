
#include "y/chemical/plexus/cluster/conservations.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

#include "y/apex/api/ortho/coven/survey/natural.hpp"
#include "y/apex/api/count-non-zero.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        using namespace MKL;
        using namespace Apex;
        using namespace Ortho;
        using namespace Coven;

        ClusterConservations:: ~ClusterConservations() noexcept
        {

        }

        ClusterConservations:: ClusterConservations( XMLog &xml, const ClusterContent::Pointer &ptr) :
        ClusterTopology(xml,ptr)
        {
            Y_XML_SECTION(xml,"ClusterConservations");

            Matrix<apz> Q;
            if(!OrthoSpace::Make(Q,topology)) {
                throw Specific::Exception(CallSign, "no orthogonal space (singular equilibri%s)", Plural::aum(N));
            }

            const NaturalSurvey survey(xml,Q,0);
            if(survey->size<=0) {
                Y_XMLOG(xml,"no conservation");
                return;
            }

            {
                uMatrix &Cm = Coerce(preserved);
                Cm.make(survey->size,M);


                size_t cidx = 1;
                for(const NaturalSurvey::ArrayType *node=survey->head;node;node=node->next,++cidx)
                {
                    const Readable<apn> &coef = *node; assert(CountNonZero(coef)>=2);
                    Writable<unsigned>  &cons = Cm[cidx];
                    for(size_t j=M;j>0;--j) {
                        if( !coef[j].tryCast(cons[j]))
                            throw Specific::Exception(CallSign, "conservation coefficient overflow");
                    }
                }
            }

            Y_XMLOG(xml, "topology  = " << topology);
            Y_XMLOG(xml, "preserved = " << preserved);

        }


#if 0
        void Cluster:: conservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "conservations");

            Matrix<apz> Q;
            if(!OrthoSpace::Make(Q,topology)) {
                throw Specific::Exception(CallSign, "no orthogonal space");
            }

            const NaturalSurvey survey(xml,Q,0);
            if(survey->size<=0) {
                Y_XMLOG(xml,"no conservation");
                return;
            }

            {
                uMatrix &Cm = Coerce(preserved);
                const size_t M = my.species->size;
                Cm.make(survey->size,M);


                size_t cidx = 1;
                for(const NaturalSurvey::ArrayType *node=survey->head;node;node=node->next,++cidx)
                {
                    const Readable<apn> &coef = *node; assert(CountNonZero(coef)>=2);
                    Writable<unsigned>  &cons = Cm[cidx];
                    for(size_t j=M;j>0;--j) {
                        if( !coef[j].tryCast(cons[j]))
                            throw Specific::Exception(CallSign, "conservation coefficient overflow");
                    }
                }
            }

            Y_XMLOG(xml, "topology  = " << topology);
            Y_XMLOG(xml, "preserved = " << preserved);


        }
#endif

    }

}

