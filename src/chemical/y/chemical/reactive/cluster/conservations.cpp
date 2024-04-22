
#include "y/chemical/reactive/cluster.hpp"
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/natural.hpp"
#include "y/apex/mylar.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        void Cluster:: buildConservations(XMLog &xml)
        {
            Y_XML_SECTION(xml,"Combinatorics");
            //const size_t n = Nu.rows; assert(n>0);

            WOVEn::NaturalSurvey survey(xml);
            {
                Matrix<apz> Q;
                if(!MKL::OrthoSpace::Make(Q,Nu))
                    throw Specific::Exception("Cluster::BuildConservations", "singular topology orthogonal space");
                Y_XMLOG(xml, "NuOrtho=" << Q);
                WOVEn::Explore(Q,survey,true);
            }
            Y_XMLOG(xml,"#conservation = " << survey.size);
            if(survey.size>0)
            {
                const size_t      m = Nu.cols; assert(m>0);
                Matrix<unsigned> &Q = Coerce(Qm);

                Q.make(survey.size,m);
                survey.sort();
                size_t            i=0;
                for(const WOVEn::NaturalArray *arr=survey.head;arr;arr=arr->next)
                {
                    Writable<unsigned>        &q = Q[++i];
                    const Readable<const apn> &a = *arr;
                    for(size_t j=m;j>0;--j)    q[j] = a[j].cast<unsigned>("conservation");
                }

                Y_XMLOG(xml, "Qm=" << Q);
            }


        }



    }

}

