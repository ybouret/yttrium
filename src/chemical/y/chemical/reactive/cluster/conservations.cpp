#include "y/chemical/reactive/cluster/conservations.hpp"
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/natural.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        ClusterConservations:: ~ClusterConservations() noexcept
        {
        }

        ClusterConservations:: ClusterConservations(const Fragment    &fragment,
                                                    const Constants   &topK,
                                                    XMLog             &xml) :
        ClusterTopology(fragment,topK,xml),
        Qm(),
        laws(),
        groups()
        {
            static const char here[] = "Cluster::Conservations";
            Y_XML_SECTION(xml,here);
            const size_t m = Nu.cols; assert(m>0);

            //------------------------------------------------------------------
            //
            // use woven to explore Nu orthogonal space
            //
            //------------------------------------------------------------------
            {

                WOVEn::NaturalSurvey survey(xml);
                {
                    Matrix<apz> Q;
                    if(!MKL::OrthoSpace::Make(Q,Nu))
                        throw Specific::Exception("Cluster::BuildConservations", "singular topology orthogonal space");
                    Y_XMLOG(xml, "NuOrtho=" << Q);
                    WOVEn::Explore(Q,survey,true);
                }
                Y_XMLOG(xml,"#conservation = " << survey.size);
                if(survey.size<=0) return;

                //--------------------------------------------------------------
                // build conservation matrix from survey
                //--------------------------------------------------------------
                Matrix<unsigned> &Q = Coerce(Qm);
                Q.make(survey.size,m);
                survey.sort();
                {
                    size_t            i=0;
                    for(const WOVEn::NaturalArray *arr=survey.head;arr;arr=arr->next)
                    {
                        Writable<unsigned>        &q = Q[++i];
                        const Readable<const apn> &a = *arr;
                        for(size_t j=m;j>0;--j)    q[j] = a[j].cast<unsigned>("conservation");
                    }
                }
                Y_XMLOG(xml, "Nu=" << Nu);
                Y_XMLOG(xml, "Qm=" << Q);
            }

            //------------------------------------------------------------------
            //
            // create conservations laws
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml,"WriteDown");
                Coerce(laws).writeDown(Qm,spset);
                if(xml.verbose)
                {
                    for(const Conservation::Law *law=laws->head;law;law=law->next)
                    {
                        xml() << *law << std::endl;
                    }
                }
            }

            //------------------------------------------------------------------
            //
            // create independent groups of dependant laws
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml,"Collect");
                Coerce(groups).collect(laws);
                if(xml.verbose)
                {
                    xml()  << " (*) #group=" << groups->size << std::endl;
                    for(const Conservation::Group *grp=groups->head;grp;grp=grp->next)
                    {
                        xml() << *grp << std::endl;
                    }
                }
            }
        }
    }

}

#include "y/chemical/reactive/cluster.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        void Cluster:: buildConservations(XMLog &xml)
        {
            Y_XML_SECTION(xml,"Conservations");
            const size_t m = Nu.cols; assert(m>0);

            //------------------------------------------------------------------
            //
            // use woven to explore Nu orthogonal space
            //
            //------------------------------------------------------------------
            {

                WOVEn::NaturalSurvey survey(xml);
                {
                    Matrix<apz> Q;
                    if(!MKL::OrthoSpace::Make(Q,Nu))
                        throw Specific::Exception("Cluster::BuildConservations", "singular topology orthogonal space");
                    Y_XMLOG(xml, "NuOrtho=" << Q);
                    WOVEn::Explore(Q,survey,true);
                }
                Y_XMLOG(xml,"#conservation = " << survey.size);
                if(survey.size<=0) return;

                //--------------------------------------------------------------
                // build conservation matrix from survey
                //--------------------------------------------------------------
                Matrix<unsigned> &Q = Coerce(Qm);
                Q.make(survey.size,m);
                survey.sort();
                {
                    size_t            i=0;
                    for(const WOVEn::NaturalArray *arr=survey.head;arr;arr=arr->next)
                    {
                        Writable<unsigned>        &q = Q[++i];
                        const Readable<const apn> &a = *arr;
                        for(size_t j=m;j>0;--j)    q[j] = a[j].cast<unsigned>("conservation");
                    }
                }
                Y_XMLOG(xml, "Nu=" << Nu);
                Y_XMLOG(xml, "Qm=" << Q);
            }

            //------------------------------------------------------------------
            //
            // create conservations laws
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml,"WriteDown");
                Coerce(claws).writeDown(Qm,spset);
                if(xml.verbose)
                {
                    for(const Conservation::Law *law=claws->head;law;law=law->next)
                    {
                        xml() << *law << std::endl;
                    }
                }
            }

            //------------------------------------------------------------------
            //
            // create independent groups of dependant laws
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml,"Collect");
                Coerce(cgrps).collect(claws);
                if(xml.verbose)
                {
                    xml()  << " (*) #group=" << cgrps->size << std::endl;
                    for(const Conservation::Group *grp=cgrps->head;grp;grp=grp->next)
                    {
                        xml() << *grp << std::endl;
                    }
                }
            }
            
        }



    }

}

