
#include "y/chem/plexus/cluster.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        static inline unsigned CountConserved(const Actors      &actors,
                                            const AddressBook &conserved) noexcept
        {
            unsigned res = 0;
            for(const Actor *a=actors.head;a;a=a->next)
            {
                if( conserved.search( a->sp) )
                    ++res;
            }
            return res;
        }
        
        void Cluster:: createArmy(XMLog &xml)
        {

            Y_XML_SECTION(xml, "Cluster::CreateArmy");
            size_t       i = 1;
            for(const EqNode *node=all.head;node;node=node->next,++i)
            {
                const Equilibrium &eq = **node;
                if(i!=eq.indx[SubLevel])
                    throw Specific::Exception("Chemical::Cluster", "<%s> bad id!", eq.name.c_str());

                //--------------------------------------------------------------
                //
                // first pass: kind of equilibrium
                //
                //--------------------------------------------------------------
                static const unsigned HasNone = 0x00;
                static const unsigned HasReac = 0x01;
                static const unsigned HasProd = 0x02;
                static const unsigned HasBoth = HasReac | HasProd;
                {
                    unsigned           flag  = HasNone;
                    if(eq.reac.size>0) flag |= HasReac;
                    if(eq.prod.size>0) flag |= HasProd;

                    switch( flag )
                    {
                        case HasNone:
                            throw Specific::Exception(eq.name.c_str(), "unexpected empty equilibrium");

                        case HasProd:
                            Y_XMLOG(xml, " (*) prodOnly " << eq );
                            Coerce(army->prodOnly) << eq;
                            continue;

                        case HasReac:
                            Y_XMLOG(xml, " (*) reacOnly " << eq);
                            Coerce(army->reacOnly) << eq;
                            continue;

                        case HasBoth:
                            break;
                    }
                    assert(HasBoth == flag);
                }

                //--------------------------------------------------------------
                //
                // second pass : for both ways equilibirum
                //
                //--------------------------------------------------------------
                {
                    unsigned            conservedFlag = HasNone;;
                    const unsigned      conservedReac = CountConserved(eq.reac,tier->conserved);
                    const unsigned      conservedProd = CountConserved(eq.prod,tier->conserved);
                    if(conservedReac>0) conservedFlag |= HasReac;
                    if(conservedProd>0) conservedFlag |= HasProd;

                    switch( conservedFlag )
                    {
                        case HasNone:
                            Y_XMLOG(xml, " (*) spurious " << eq);
                            Coerce(army->spurious) << eq;
                            break;

                        case HasReac:
                        case HasProd:
                            throw Specific::Exception(eq.name.c_str(), "algebraic error #conserved reac=%u/prod=%u", conservedReac, conservedProd);

                        case HasBoth:
                            Y_XMLOG(xml, " (*) definite " << eq);
                            Coerce(army->definite) << eq;
                            break;
                    }

                }


            }
            Y_XMLOG(xml,"  |-#ProdOnly: " << army->prodOnly.size);
            Y_XMLOG(xml,"  |-#ReacOnly: " << army->reacOnly.size);
            Y_XMLOG(xml,"  |-#Spurious: " << army->spurious.size);
            Y_XMLOG(xml,"  |-#Definite: " << army->definite.size);


        }
    }

}

