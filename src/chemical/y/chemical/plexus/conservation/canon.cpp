
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
            iAlpha(),
            xAlpha(),
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
#include "y/mkl/algebra/lu.hpp"
#include "y/counting/combination.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            using namespace MKL;


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
                {
                    size_t auxId = 0;
                    for(const LNode *l=head;l;l=l->next)
                    {
                        const Law &law = **l;
                        enroll( law );
                        Coerce(law.auxId) = ++auxId;
                        Y_XMLOG(xml,"(+) " <<law.name);
                        for(const Actor *a=law->head;a;a=a->next)
                        {
                            Coerce(species) << a->sp;
                            Coerce(sformat).enroll(a->sp);
                        }
                    }
                }



                Y_XML_COMMENT(xml, std::setw(4) << species->size << " = # " << species);

                //--------------------------------------------------------------
                //
                //
                // compute the rank of the local matrices
                //
                //
                //--------------------------------------------------------------
                {
                    iMatrix    &iA = Coerce(iAlpha);
                    XMatrix    &xA = Coerce(xAlpha);
                    iA.make(size,species->size);
                    xA.make(size,species->size);
                    for(const LNode *ln=head;ln;ln=ln->next)
                    {
                        const Law &law = **ln;
                        law.iFillArray(iA[law.auxId], AuxLevel);
                        law.xFillArray(xA[law.auxId], AuxLevel);
                    }
                    //Y_XMLOG(xml, "A   =" << iA);
                    Coerce(rank) = Rank::Of(iA);
                    //Y_XMLOG(xml, "rank=" << rank);
                    Y_XML_COMMENT(xml, std::setw(4) << rank << " = rank");
                }

                //--------------------------------------------------------------
                //
                //
                // collect concerned definite into anxious
                //
                //
                //--------------------------------------------------------------
                for(const ENode *en=definite.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(eq.gotAnyOf(*species))
                    {
                        Coerce(anxious) << eq;
                        Y_XMLOG(xml, "(*) " << eq);
                    }
                }
                Y_XML_COMMENT(xml, std::setw(4) << anxious->size <<  " = #anxious");

                {
                    const size_t n = size;
                    const size_t k = rank;
                    Combination comb(n,k);
                    std::cerr << "#matrices(" << n << "," << k << ")=" << comb.total << std::endl;
                }

            }

        }

    }

}
