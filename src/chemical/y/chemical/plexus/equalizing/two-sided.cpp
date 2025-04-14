#include "y/chemical/plexus/equalizing/two-sided.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {
            TwoSided:: TwoSided(const Cluster               &_cluster,
                                const Conservation::Canon   &_canon,
                                const Banks                 &_banks,
                                const GBank                 &_gbank) :
            cluster( _cluster ),
            canon( _canon ),
            probe( _banks),
            nrows( canon.anxious->size ),
            ncols( cluster->species->size),
            zgain( new GList(_gbank) ),
            pgain( new GList(_gbank) ),
            klist( new KList()      ),
            c_eqz(nrows,ncols),
            xadd(),
            next(0),
            prev(0)
            {
            }


            TwoSided:: ~TwoSided() noexcept {}

            //! by decreasing gain then by increasing index
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
                                Summator          &II,
                                const AddressBook &vanishing)
            {

                Y_XML_SECTION(xml,"Equalizing::TwoSided");
                if(xml.verbose) vanishing.display<Species>(xml() << "vanishing: ") << std::endl;


                {
                    const AddressBook &wanders = cluster.wandering;
                    size_t cycle = 0;
                CYCLE:
                    ++cycle;
                    zgain->free();
                    pgain->free();
                    klist->free();
                    for(const ENode *en=cluster.definite->head;en;en=en->next)
                    {
                        //______________________________________________________
                        //
                        //
                        // probe status
                        //
                        //______________________________________________________
                        const Equilibrium &eq   = **en;
                        const Resultant    res  = probe(xml,eq,C0,L0,wanders);

                        //______________________________________________________
                        //
                        //
                        // take action
                        //
                        //______________________________________________________
                        switch(res)
                        {
                            case Correct: continue;              // nothing to do
                            case BadBoth: *klist << eq; continue; // blocked...
                            case BadReac: break;
                            case BadProd: break;
                        }

                        //______________________________________________________
                        //
                        //
                        // prepare a correction and dispatch according to gain
                        //
                        //______________________________________________________
                        XWritable &cc = cluster.transfer(c_eqz[zgain->size+pgain->size+1],SubLevel,C0,L0);
                        const Gain G(probe.generate(xml,xadd,cc,eq,C0,L0,wanders),eq,cc); assert(G.g.mantissa>=0);
                        if(G.g.mantissa<=0)
                            *zgain << G;
                        else
                            *pgain << G;
                    }

                    {
                        Y_XML_SECTION_OPT(xml,"Status","cycle=" << cycle);
                        MergeSort::Call(*pgain,CompareGains);
                        klist->show(xml, Sign::ToText(Negative) );
                        zgain->show(xml, Sign::ToText(__Zero__) );
                        pgain->show(xml, Sign::ToText(Positive) );
                    }

                    //______________________________________________________
                    //
                    //
                    // apply the best gain
                    //
                    //______________________________________________________
                    if(pgain->size>0)
                    {
                        const Gain &winner = **pgain->head;
                        cluster.transfer(C0,L0,winner.C,SubLevel);
                        goto CYCLE;
                    }

                }

                //______________________________________________________________
                //
                //
                // no more positive gain, apply vanishing
                //
                //______________________________________________________________
                {
                    const xreal_t zero;
                    for(const SNode *sn=canon.species->head;sn;sn=sn->next)
                    {
                        const Species & sp = **sn;
                        xreal_t       & cc = sp(C0,L0);
                        if(vanishing.has(sp) || cc < zero)
                        {
                            sp(II,SubLevel) << -cc;
                            cc = zero;
                        }
                        assert(cc>=zero);
                    }
                }

                cluster.show(std::cerr << "Ceqz=", L0, "\t[", C0, "]", xreal_t::ToString) << std::endl;
                cluster.show(std::cerr << "Cinj=", SubLevel, "\t[", II, "]") << std::endl;



            }

        }

    }

}

