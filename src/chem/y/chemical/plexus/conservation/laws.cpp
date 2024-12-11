
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

            Laws::ConstInterface & Laws:: surrogate() const noexcept { return my; }

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
                            crew.pushTail(new Actor(nu,sp) );
                        }
                        assert(crew.size>=2);

                        const Law & law = *my.pushTail( new Law(crew) );
                        my.enroll(*law);
                        Coerce(law.place) = ic;
                        Y_XMLOG(xml,"(+) " << law);
                    }
                }

                Y_XMLOG(xml, "conservancy=" << mix.conservancy );




#if 0
                {
                    Matrix<unsigned> &cmtx = Coerce(cluster.conservancy); assert(0==cluster.conservancy.rows);
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

                Y_XMLOG(xml, "conservancy=" << cluster.conservancy );
#endif

            }

        }

    }

}

