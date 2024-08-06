#include "y/chemical/plexus/solver/normalizer.hpp"

#include "y/mkl/opt/minimize.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        


        size_t Normalizer:: extract(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Extract");

            //--------------------------------------------------------------
            //
            // reset applicant/ortho family
            //
            //--------------------------------------------------------------
            apl.free();
            qfm.free();

            //--------------------------------------------------------------
            //
            // loop over applicants
            //
            //--------------------------------------------------------------
            const size_t nap = aps.size();
            for(size_t i=1;i<=nap;++i)
            {
                const Applicant &app = aps[i];
                if( qfm.wouldAccept( rcl.topology[ app.isub() ] ) )
                {
                    apl << app;
                    qfm.expand();
                    if( apl.size >= dof) break; // base is filled
                }
            }

            //--------------------------------------------------------------
            //
            // found the base: order by equilibria top-level
            // to build local topology
            //
            //--------------------------------------------------------------
            //MergeSort::Call(apl,CompareBasis);




            //--------------------------------------------------------------
            //
            // summary
            //
            //--------------------------------------------------------------
            assert(apl.size <= Min(dof,nap) );
            Y_XMLOG(xml, "#applicant  = " << aps.size());
            Y_XMLOG(xml, "#primary    = " << dof);
            Y_XMLOG(xml, "#family     = " << apl.size);

#if 0
            {
                XMatrix &Nu = XNu[apl.size];
                size_t   ii = 0;
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    const Applicant &app = **an;
                    app.eq.topology(Nu[++ii],SubLevel);
                    if(xml.verbose)
                    {
                        app.display( xml() << "| ", cl.uuid, false) << std::endl;
                    }
                }
                Y_XMLOG(xml, "Nu=" << Nu);
            }
#endif

            //--------------------------------------------------------------
            //
            // return apl.size <= min(dof,nap)
            //
            //--------------------------------------------------------------
            return apl.size;
        }

    }

}


