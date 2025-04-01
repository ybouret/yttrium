
#include "y/chemical/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {


            Canon:: Canon(const Law &first) :
            LList(),
            rank(0),
            species(),
            anxious(),
            sformat(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }

            Canon:: ~Canon() noexcept
            {
            }

            bool Canon:: accepts(const Law &law) const noexcept
            {
                for(const LNode *ln=head;ln;ln=ln->next)
                {
                    if( (**ln).hasCommonActorWith(law) ) return true;
                }
                return false;
            }


            bool Canon:: accepts(const Canon &rhs) const noexcept
            {
                for(const LNode *l=head;l;l=l->next)
                {
                    if(rhs.accepts(**l)) return true;
                }
                return false;
            }

        }

    }

}

#include "y/text/plural.hpp"
#include "y/mkl/algebra/rank.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            void Canon:: compile(XMLog       & xml,
                                 const EList & definite)
            {
                Y_XML_SECTION_OPT(xml, "Conservation::Canon", "size=" << size);
                assert( 0 == maxNameLength );
                assert( 0 == species->size );

                //--------------------------------------------------------------
                //
                //
                // collect species and make AuxLevel
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "law" << Plural::s(size));
                for(const LNode *l=head;l;l=l->next)
                {
                    const Law &law = **l; enroll( law );
                    Y_XMLOG(xml,"(+) " <<law.name);
                    for(const Actor *a=law->head;a;a=a->next)
                    {
                        Coerce(species) << a->sp;
                        Coerce(sformat).enroll(a->sp);
                    }
                }



                Y_XML_COMMENT(xml, "species");;
                Y_XMLOG(xml, "(#) " << species);

                //--------------------------------------------------------------
                //
                //
                // compute the rank of the local matrix
                //
                //
                //--------------------------------------------------------------
                {
                    Y_XML_COMMENT(xml, "rank");
                    Matrix<apq> Q(size,species->size);
                    {
                        size_t i=1;
                        for(const LNode *ln=head;ln;ln=ln->next,++i)
                        {
                            const Law &law = **ln;
                            for(const Actor *a=law->head;a;a=a->next)
                            {
                                a->sp(Q[i],AuxLevel) = a->nu;
                            }
                        }
                    }
                    Y_XMLOG(xml, "Q   =" << Q);
                    Coerce(rank) = MKL::Rank::Compute(Q);
                    Y_XMLOG(xml, "rank=" << rank);
                }

                //--------------------------------------------------------------
                //
                //
                // compute all possibilities
                //
                //
                //--------------------------------------------------------------
                {
                    apn total = 0;
                    for(size_t k=1;k<=rank;++k)
                    {
                        const apn nk = apn::Comb(size,k);
                        Y_XML_COMMENT(xml,"projection@rank" << k << " : " << nk);
                    }

                }



                //--------------------------------------------------------------
                //
                //
                // collect definite into anxious
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "anxious");
                for(const ENode *en=definite.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(eq.gotAnyOf(*species))
                    {
                        Coerce(anxious) << eq;
                        Y_XMLOG(xml, "(*) " << eq);
                    }
                }

            }

        }

    }

}
