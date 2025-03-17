
#include "y/chemical/plexus/cluster/conservations.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"

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
        ClusterTopology(xml,ptr),
        preserved(),
        ordinance(0),
        conserved(ordinance.species),
        unbounded()
        {
            Y_XML_SECTION(xml,"ClusterConservations");

            {
                CxxListOf<Conservation::Rule> rules;
                {
                    Matrix<apz> Q;
                    if(!OrthoSpace::Make(Q,topology))
                        throw Specific::Exception(CallSign, "no orthogonal space (singular equilibri%s)", Plural::aum(N));


                    const NaturalSurvey survey(xml,Q,0);
                    if(survey->size<=0) {
                        Y_XMLOG(xml,"no conservation found");
                        return;
                    }

                    {
                        uMatrix &Cm = Coerce(preserved);
                        Cm.make(survey->size,M);


                        size_t                cidx = 1;
                        for(const NaturalSurvey::ArrayType *node=survey->head;node;node=node->next,++cidx)
                        {
                            const Readable<apn> &       coef = *node; assert(CountNonZero(coef)>=2);
                            Writable<unsigned>  &       cons = Cm[cidx];
                            AutoPtr<Conservation::Rule> claw = new Conservation::Rule();
                            for(const SNode *sn = (*this)->species->head; sn; sn=sn->next)
                            {
                                const Species &sp = **sn;
                                const size_t   sj = sp.indx[SubLevel];
                                unsigned &     cf = cons[sj];
                                if( !coef[sj].tryCast(cf) )
                                    throw Specific::Exception(CallSign, "conservation coefficient overflow for [%s]", sp.name->c_str());
                                if(0!=cf)
                                {
                                    (*claw)(cf,sp);
                                }
                            }
                            std::cerr << " (+) " << claw->name << std::endl;
                            rules.pushTail(claw.yield());
                        }
                    }
                }
                Conservation::Laws laws(rules.head);
                Coerce(ordinance).xch(laws);

            }

            for(const SNode *sn=(*this)->species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(!conserved.has(sp)) Coerce(unbounded) << sp;
            }
            assert(conserved.isStrictlySortedBy(MetaList<SList>::Compare));
            assert(unbounded.isStrictlySortedBy(MetaList<SList>::Compare));

            Y_XMLOG(xml, "topology  = " << topology);
            Y_XMLOG(xml, "preserved = " << preserved);
            Y_XMLOG(xml, "ordinance = " << ordinance);
            Y_XMLOG(xml, "conserved = " << conserved);
            Y_XMLOG(xml, "unbounded = " << unbounded);
        }


        
        bool ClusterConservations:: areAllUnbounded(const Actors &A) const noexcept
        {
            for(const Actor *a=A->head;a;a=a->next)
            {
                if( conserved.has(a->sp) ) return false;
                assert( unbounded.has(a->sp));
            }
            return true;
        }

        ComponentsTier ClusterConservations:: tierOf(const Components &eq) const noexcept
        {
            if(eq.reac->size>0)
            {
                if(eq.prod->size>0)
                {

                }
                else
                {
                    assert(areAllUnbounded(eq.reac));
                    return ReacOnly;
                }
            }
            else
            {
                if(eq.prod->size>0)
                {

                    assert(areAllUnbounded(eq.prod));
                    return ProdOnly;
                }
                else
                {
                    return Deserted;
                }
            }
        }
    }

}

