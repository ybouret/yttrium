#include "y/chemical/plexus/equalizer/two-sided.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
        {

            std::ostream & operator<<(std::ostream &os, const Gain &gain)
            {
                os << std::setw(Restartable::Width) << gain.g.str() << " @" << gain.E.name << "=";
                gain.E.displayCompact(os,gain.C,SubLevel);
                return os;
            }


            //! decreasing gain then increasing index
            static inline
            SignType CompareGains(const GNode * const lhs,
                                  const GNode * const rhs) noexcept
            {
                const Gain &L = **lhs;
                const Gain &R = **rhs;
                switch( Sign::Of(L.g,R.g) )
                {
                    case Positive: return Negative;
                    case Negative: return Positive;
                    case __Zero__: break;
                }
                return Sign::Of(L.E.indx[TopLevel],R.E.indx[TopLevel]);
            }


            void TwoSided:: fix(XMLog             &xml,
                                XWritable         &C0,
                                const Level        L0,
                                const AddressBook &vanishing)
            {

                Y_XML_SECTION(xml,"Equalizer::Ordinary");
                if(xml.verbose)
                {
                    vanishing.display<Species>(xml() << "vanishing: ") << std::endl;
                }


                size_t cycle = 0;
            CYCLE:
                ++cycle;
                Y_XML_COMMENT(xml, "[[ Fix Cycle #" << std::setw(4) << cycle << " ]]");
                zgain.free();
                pgain.free();
                klist.free();
                for(const ENode *en=cluster.definite->head;en;en=en->next)
                {
                    //__________________________________________________________
                    //
                    //
                    // probe status
                    //
                    //__________________________________________________________
                    const Equilibrium &eq   = **en;
                    const Resultant    res  = probe(xml,eq,C0,L0, & cluster.wandering );

                    //__________________________________________________________
                    //
                    //
                    // take action
                    //
                    //__________________________________________________________
                    switch(res)
                    {
                        case Correct: continue;              // nothing to do
                        case BadBoth: klist << eq; continue; // blocked...
                        case BadReac: break;
                        case BadProd: break;
                    }

                    //__________________________________________________________
                    //
                    //
                    // prepare a correction and dispatch according to gain
                    //
                    //__________________________________________________________
                    XWritable &cc = c_eqz[zgain.size+pgain.size+1];
                    cluster.transfer(cc,SubLevel,C0,L0);
                    const Gain G(probe.generate(xml,xadd,cc,eq,C0,L0, &cluster.wandering),eq,cc); assert(G.g.mantissa>=0);
                    if(G.g.mantissa<=0)
                        zgain << G;
                    else
                        pgain << G;
                }

                {
                    Y_XML_SECTION(xml,"Status");
                    MergeSort::Call(pgain,CompareGains);
                    klist.show(xml, Sign::ToText(Negative) );
                    zgain.show(xml, Sign::ToText(__Zero__) );
                    pgain.show(xml, Sign::ToText(Positive) );
                }

                //______________________________________________________________
                //
                //
                // apply the best gain
                //
                //______________________________________________________________

                if(pgain.size>0)
                {
                    const Gain &winner = **pgain.head;
                    cluster.transfer(C0,L0,winner.C,SubLevel);
                    goto CYCLE;
                }

                cluster.show(std::cerr << "Ceqz=", L0, "[", C0, "]", xreal_t::ToString) << std::endl;

                if(klist.size||zgain.size) throw Exception("Equalize: not finished");


            }

        }

    }

}

