
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
            uAlpha(),
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

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            using namespace MKL;


            
            void Canon:: compile(XMLog       & xml,
                                 const EList & definite,
                                 const SList & unbounded)
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
                    uMatrix    &uA = Coerce(uAlpha);
                    XMatrix    &xA = Coerce(xAlpha);
                    uA.make(size,species->size);
                    xA.make(size,species->size);
                    for(const LNode *ln=head;ln;ln=ln->next)
                    {
                        const Law &law = **ln;
                        law.iFillArray(uA[law.auxId], AuxLevel);
                        law.xFillArray(xA[law.auxId], AuxLevel);
                    }
                    //Y_XMLOG(xml, "A   =" << iA);
                    Coerce(rank) = Rank::Of(uA);
                    //Y_XMLOG(xml, "rank=" << rank);
                    Y_XML_COMMENT(xml, std::setw(4) << rank << " = rank");
                }

                const SList authorized;
                Coerce(authorized) << unbounded << *species;
                MetaList<SList>::Sort( Coerce(authorized)  );

                //std::cerr << "Authorized=" << authorized << std::endl;

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
                    if(eq.madeFrom(authorized))
                    {
                        Coerce(anxious) << eq;
                        Y_XMLOG(xml, "(*) " << eq);
                    }
                }
                Y_XML_COMMENT(xml, std::setw(4) << anxious->size <<  " = #anxious");


            }

        }

    }

}
