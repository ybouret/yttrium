
#include "y/chemical/plexus/conservation/laws.hpp"
#include "y/chemical/plexus/mix.hpp"
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

            Y_PROXY_IMPL(Laws,my);

            const char * const Laws:: CallSign = "Chemical::Conservation::Laws";
            
            Laws:: Laws(Mix &mix, XMLog &xml):
            Object(),
            Proxy<const Canon>(),
            my()
            {
                Y_XML_SECTION(xml,CallSign);

                Y_XMLOG(xml, "Nu = " << mix->topology);
                AutoPtr< WOVEn::ArraysOf<unsigned> > conservations;
                {
                    Y_XML_SECTION_OPT(xml, "WOVEn::Explore","orthogonal of topology");

                    // make Natural survey
                    WOVEn::NaturalSurvey survey(xml);
                    {
                        Matrix<apz> Q;
                        if(!OrthoSpace::Make(Q,mix->topology))
                            throw Specific::Exception(CallSign,"no orthogonal space of topology");
                        Y_XMLOG(xml, "Q  = " << Q);
                        WOVEn::Explore(Q,survey,true);
                    }
                    survey.sort();

                    // conversion to unsigned arrays
                    conservations = new WOVEn::ArraysOf<unsigned>(survey,"conservation weight",2);
                }



                Y_XMLOG(xml,"#conservations=" << conservations->size);
                if(conservations->size<=0) return;

                {
                    Matrix<unsigned> &conservancy = Coerce(mix.conservancy);
                    conservancy.make(conservations->size,mix->species.size);

                    // build each row/law
                    size_t ic=1;
                    for(const WOVEn::ArrayOf<unsigned> *arr=conservations->head;arr;arr=arr->next,++ic)
                    {
                        const Readable<unsigned> &coef = conservancy[ic].ld( *arr );
                        Actor::List               crew;
                        for(const SNode *sn=mix->species.head;sn;sn=sn->next)
                        {
                            const Species &sp = **sn;
                            const unsigned nu = sp(coef,SubLevel);
                            if(nu<=0) continue;
                            crew.pushTail(new Actor(nu,sp,NameAsConcentration) );
                        }
                        assert(crew.size>=2);

                        const Law & law = *my.pushTail( new Law(crew) );
                        my.enroll(*law);
                        Coerce(law.place) = ic;
                        Y_XMLOG(xml,"(+) " << law);
                    }
                }

                Y_XMLOG(xml, "conservancy=" << mix.conservancy );



            }

        }

    }

}

