
#include "y/chemical/plexus/cluster/conservations.hpp"
#include "y/chemical/plexus/conservation/rule.hpp"

#include "y/mkl/algebra/ortho-space.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/system/exception.hpp"

#include "y/apex/api/ortho/architect.hpp"


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


        ClusterConservations:: ClusterConservations(XMLog &xml, const ClusterContent::Pointer &ptr) :
        ClusterTopology(xml,ptr),
        preserved(),
        ordinance(0),
        conserved(ordinance.species),
        unbounded(),
        wandering()
        {
            Y_XML_SECTION(xml,"ClusterConservations");

            {
                CxxListOf<Conservation::Rule> rules;
                {
                    Matrix<apz> Q;
                    if(!OrthoSpace::Make(Q,topology))
                        throw Specific::Exception(CallSign, "no orthogonal space (singular equilibri%s)", Plural::aum(N));


                    const NaturalSurvey survey(xml,Q,2,NULL);
                    if(survey->size<=0) {
                        Y_XMLOG(xml,"no conservation found");
                        goto FINALIZE;
                    }

#if 0
                    {

                        size_t  cidx=1;
                        uMatrix Cm(survey->size,M);
                        for(const NaturalSurvey::ArrayType *node=survey->head;node;node=node->next,++cidx)
                        {
                            const NaturalSurvey::ArrayType  & coef = *node; assert(CountNonZero(coef)>=2);
                            Writable<unsigned>              & cons = Cm[cidx];
                            for(const SNode *sn = (*this)->species->head; sn; sn=sn->next)
                            {
                                const Species &sp = **sn;
                                const size_t   sj = sp.indx[SubLevel];
                                unsigned &     cf = cons[sj];
                                if( !coef[sj].tryCast(cf) )
                                    throw Specific::Exception(CallSign, "conservation coefficient overflow for [%s]", sp.name->c_str());
                            }
                            Y_XMLOG(xml,"(+) " << coef);
                        }

                        std::cerr << "Cm="   << Cm << std::endl;
                        const size_t  CmRank = MKL::Rank::Of(Cm);
                        std::cerr << "rank=" << CmRank << std::endl;


                        std::cerr << sizeof(Coven::SArray<apn>) << std::endl;
                        std::cerr << sizeof(Coven::SArray<apz>) << std::endl;

                        Ortho::Architect architect(M);
                        uMatrix basis;
                        architect.extract(basis,Cm,CmRank);
                        std::cerr << "basis=" << basis << std::endl;


                        throw Exception("must study survey");
                    }
#endif


                    {
                        uMatrix &Cm = Coerce(preserved);
                        Cm.make(survey->size,M);


                        size_t                cidx = 1;
                        for(const NaturalSurvey::ArrayType *node=survey->head;node;node=node->next,++cidx)
                        {
                            const Readable<apn> &       coef = *node; assert(CountNonZero(coef)>=2);
                            Writable<unsigned>  &       cons = Cm[cidx];
                            AutoPtr<Conservation::Rule> rule = new Conservation::Rule();
                            for(const SNode *sn = (*this)->species->head; sn; sn=sn->next)
                            {
                                const Species &sp = **sn;
                                const size_t   sj = sp.indx[SubLevel];
                                unsigned &     cf = cons[sj];
                                if( !coef[sj].tryCast(cf) )
                                    throw Specific::Exception(CallSign, "conservation coefficient overflow for [%s]", sp.name->c_str());
                                if(0!=cf)
                                {
                                    (*rule)(cf,sp);
                                }
                            }
                            Y_XMLOG(xml,"(+) " << rule->name);
                            rules.pushTail(rule.yield());
                        }
                    }
                }
                Conservation::Laws laws(rules.head);
                Coerce(ordinance).xch(laws);
            }

        FINALIZE:
            for(const SNode *sn=(*this)->species->head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                if(!conserved.has(sp)) Coerce(unbounded) << sp;
            }
            assert(conserved.isStrictlySortedBy(MetaList<SList>::Compare));
            assert(unbounded.isStrictlySortedBy(MetaList<SList>::Compare));

            for(const SNode *sn=unbounded.head;sn;sn=sn->next)
            {
                Coerce(wandering) += **sn;
            }

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
                if( conserved.has(a->sp) ) {
                    assert( !unbounded.has(a->sp) );
                    return false;
                }
                assert( unbounded.has(a->sp) );
            }
            return true;
        }

        bool   ClusterConservations:: gotAnyConserved(const Actors &A) const noexcept
        {
            for(const Actor *a=A->head;a;a=a->next)
            {
                if( conserved.has(a->sp) ) {
                    assert( !unbounded.has(a->sp) );
                    return true;
                }
                assert( unbounded.has(a->sp));
            }
            return false;
        }

        const Conservation:: Law * ClusterConservations:: preserving(Lingo::Matching &m) const
        {
            return ordinance.preserving(m);
        }
    }

}

