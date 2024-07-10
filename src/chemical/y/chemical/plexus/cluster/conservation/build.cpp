
#include "y/chemical/plexus/cluster.hpp"
#include "y/system/exception.hpp"
#include "y/woven/survey/natural.hpp"
#include "y/woven/subspaces.hpp"
#include "y/mkl/algebra/ortho-space.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        void Cluster:: buildConservations(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Conservations");

            //------------------------------------------------------------------
            //
            // preparing survey
            //
            //------------------------------------------------------------------
            WOVEn::NaturalSurvey survey(xml);

            //------------------------------------------------------------------
            //
            // survey orthogonal space of topology
            //
            //------------------------------------------------------------------
            {
                Matrix<apz> Q;
                if(! MKL::OrthoSpace::Make(Q,Nu) )
                    throw Specific::Exception("Conservations", "singular topology ortho-space");
                if(Q.cols>0)
                    WOVEn::Explore(Q,survey,true);
            }
            if(survey.size<=0) return;
            
            //------------------------------------------------------------------
            //
            // convert survey to laws
            //
            //------------------------------------------------------------------
            const size_t m = species.size;
            {
                survey.sort();
                Matrix<unsigned> &Q = Coerce(Qm);
                Q.make(survey.size,m);
                size_t i = 0;
                for(const WOVEn::NaturalArray *arr=survey.head;arr;arr=arr->next)
                {
                    Writable<unsigned>        &q = Q[++i];
                    const Readable<const apn> &a = *arr;
                    for(size_t j=m;j>0;--j)    q[j] = a[j].cast<unsigned>("conservation");
                }
            }
            Y_XML_SECTION(xml, "Summary");
            Y_XMLOG(xml, "Nu=" << Nu );
            Y_XMLOG(xml, "Qm=" << Qm );

            Coerce(laws) = new Conservation::Laws(species,Qm);
            for(const CLaw *law = laws->head;law;law=law->next)
            {
                Y_XMLOG(xml,*law);
            }
        }
    }

}
