
#include "y/chemical/reactive/conservation/laws.hpp"
#include "y/chemical/reactive/cluster.hpp"
#include "y/system/exception.hpp"
#include "y/mkl/algebra/ortho-space.hpp"

#include "y/woven/survey/natural.hpp"
#include "y/woven/subspaces.hpp"




namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

            using namespace MKL;

            Laws:: ~Laws() noexcept {}

            Laws::ConstInterface & Laws:: surrogate() const noexcept { return canon; }

            const char * const Laws:: CallSign = "Chemical::Conservation::Laws";
            
            Laws:: Laws(Cluster &cluster, XMLog &xml):
            Object(),
            Proxy<const Canon>(),
            canon()
            {
                Y_XML_SECTION(xml,CallSign);

                Y_XML_COMMENT(xml, "building orthogonal space of topology");
                Y_XMLOG(xml, "Nu = " << cluster->iTopology);
                WOVEn::NaturalSurvey survey(xml);

                {
                    Y_XML_SECTION(xml, "WOVEn::Explore");
                    Matrix<apz> Q;
                    if(!OrthoSpace::Make(Q,cluster->iTopology))
                        throw Specific::Exception(CallSign,"no orthogonal space of topology");

                    Y_XMLOG(xml, "Q  = " << Q);
                    WOVEn::Explore(Q,survey,true);
                }

                Y_XMLOG(xml,"#conservations=" << survey.size);
                if(survey.size<=0) return;

                survey.sort();
                {
                    Matrix<unsigned> &cmtx = Coerce(cluster.cmtx); assert(0==cmtx.rows);
                    cmtx.make(survey.size,cluster->species.size);

                    size_t ic=1;
                    for(const WOVEn::NaturalArray *warr=survey.head;warr;warr=warr->next,++ic)
                    {
                        const Readable<const apn> &w = *warr;
                        Writable<unsigned>        &u = cmtx[ic];
                        Actor::List                a;

                        //------------------------------------------------------
                        // extract actors from survey
                        //------------------------------------------------------
                        for(const SNode *sn=cluster->species.head;sn;sn=sn->next)
                        {
                            const Species &sp = **sn;
                            const apn     &sw = sp(w,SubLevel);
                            const unsigned nu = sw.cast<unsigned>("conservation weight");
                            if(nu<=0) continue;
                            a.pushTail( new Actor(nu,sp) );
                            sp(u,SubLevel) = nu;
                        }
                        if(a.size<2) continue;

                        //------------------------------------------------------
                        // update canon
                        //------------------------------------------------------
                        const Law & law = * canon.pushTail( new Law(a) );
                        canon.enroll(*law);
                        Coerce(law.place) = ic;
                        Y_XMLOG(xml,"(+) " << law);
                    }


                }

                Y_XMLOG(xml, "cmtx=" << cluster.cmtx );

            }

        }

    }

}

