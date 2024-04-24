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


        static inline void TransferTo(SList &target, const AddressBook &source)
        {
            SendBookTo<SList>(target,source);
            LightSort::BySubLevel(target);
        }

        ClusterConservations:: ClusterConservations(const Fragment    &fragment,
                                                    const Constants   &topK,
                                                    XMLog             &xml) :
        ClusterTopology(fragment,topK,xml),
        Qm(),
        laws(),
        groups(),
        conserved(),
        unbounded(),
        conservedSpecies(),
        unboundedSpecies()
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

                //--------------------------------------------------------------
                // create laws from Qm
                //--------------------------------------------------------------
                Coerce(laws).writeDown(Qm,spset);
                if(xml.verbose)
                {
                    for(const Conservation::Law *law=laws->head;law;law=law->next)
                    {
                        xml() << *law << std::endl;
                    }
                }

                //--------------------------------------------------------------
                // reckon species
                //--------------------------------------------------------------
                for(const SNode *sn=species.head;sn;sn=sn->next)
                {
                    const Species &sp    = **sn;
                    const size_t   j     = sp.indx[SubLevel];
                    bool           found = false;

                    for(size_t i=Qm.rows;i>0;--i)
                    {
                        if(0!=Qm[i][j]) {
                            found = true;
                            break;
                        }
                    }

                    if( found )
                    {
                        Coerce(conserved) |= sp;
                    }
                    else
                    {
                        Coerce(unbounded) |= sp;
                    }
                }
                assert(species.size == conserved.size() + unbounded.size());

                TransferTo( Coerce(unboundedSpecies), unbounded); assert(unboundedSpecies.size==unbounded.size());
                TransferTo( Coerce(conservedSpecies), conserved); assert(conservedSpecies.size==conserved.size());
                Y_XMLOG(xml, "conserved: " << conservedSpecies);
                Y_XMLOG(xml, "unbounded: " << unboundedSpecies);

            }

            //------------------------------------------------------------------
            //
            // create independent groups of dependant laws
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml,"Collect");
                Coerce(groups).collect(laws);
                Y_XMLOG(xml, " (*) #group=" << groups->size);
            }
        }
    }

}



