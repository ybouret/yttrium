
#include "y/chemical/plexus/equalizer.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Equalizer:: CallSign = "Chemical::Equalizer";

        Equalizer:: Equalizer(const Cluster &cl) :
        rcl(cl),
        neq(cl.size),
        nsp(cl.species.size),
        xadd(nsp*2),
        banks(),
        best(banks.s),
        faders(neq,CopyOf,banks),
        ceq(neq,nsp),
        ddc(neq,nsp),
        altered(neq),
        swell(nsp,CopyOf,neq)
        {
        }

        Equalizer::  ~Equalizer() noexcept {}

        size_t Equalizer:: unbalanced(const XReadable &C,
                                      const Level      L,
                                      XMLog           &xml)
        {
            size_t invalid = 0;
            {
                const AddressBook &book = rcl.conserved.book;
                altered.free();
                for(const ENode *en=rcl.limited.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    Faders            &fd = faders[ eq.indx[SubLevel] ];
                    const unsigned     id = fd(C,L,eq,book);
                    const size_t       ii = altered.size() + 1;
                    XWritable         &cc = ceq[ii];
                    XWritable         &dc = ddc[ii];
                    rcl.transfer(cc,SubLevel,C,L);

                    if(xml.verbose)
                    {
                        if(id!=Faders::BALANCED)
                        {
                            rcl.uuid.pad( xml() << Faders::TextFlag(id) << " |" << eq.name, eq) << "|";
                            switch (id)
                            {
                                case Faders::BALANCED:
                                    break;
                                    
                                case Faders::BAD_BOTH:
                                    *xml << "reac:" << fd.reac.required << "//prod:" << fd.prod.required;
                                    break;
                                    
                                case Faders::BAD_REAC:
                                    *xml << "reac:" << fd.reac.required << "<=prod:" << fd.prod.limiting;
                                    break;
                                    
                                case Faders::BAD_PROD:
                                    *xml << "prod:" << fd.prod.required << "<=reac:" << fd.reac.limiting;
                                    break;
                            }
                            
                            *xml << std::endl;
                        }
                    }

                    switch(id)
                    {
                        case Faders::BALANCED:
                            //--------------------------------------------------
                            //
                            // skip
                            //
                            //--------------------------------------------------
                            continue;

                        case Faders::BAD_BOTH:
                            //--------------------------------------------------
                            //
                            // stuck
                            //
                            //--------------------------------------------------
                            ++invalid;
                            continue;

                        case Faders::BAD_PROD: ++invalid; {
                            //--------------------------------------------------
                            //
                            // need a FORWARD alteration
                            //
                            //--------------------------------------------------
                            assert(fd.prod.required.size>0);
                            assert(fd.reac.limiting.size>0);
                            if(hasBestEffort(fd.reac.limiting,fd.prod.required))
                            {
                                //Y_XMLOG(xml, "\tbest: " << best);
                                addBestAlter(eq,cc,dc);
                            }
                            else
                            {
                                //Y_XMLOG(xml, "\tno best effort");
                            }
                        } continue;

                        case Faders::BAD_REAC: ++invalid; {
                            //--------------------------------------------------
                            //
                            // need a REVERSE alteration
                            //
                            //--------------------------------------------------
                            assert(fd.reac.required.size>0);
                            assert(fd.prod.limiting.size>0);
                            if(hasBestEffort(fd.prod.limiting,fd.reac.required))
                            {
                                best.xi = -best.xi;
                                //Y_XMLOG(xml, "\tbest: " << best);
                                addBestAlter(eq,cc,dc);
                            }
                            else
                            {
                                //Y_XMLOG(xml, "\tno best effort");
                            }
                        }  continue;
                    }

                    throw Specific:: Exception(CallSign,"corrupted unbalanced '%s'", eq.name.c_str());
                }
            }
            return invalid;
        }

        size_t Equalizer:: run(XWritable &C, const Level L, XMLog &xml)
        {
            Y_XML_SECTION(xml, "Eqz");
            const xreal_t      zero;

            //------------------------------------------------------------------
            //
            //
            // collect all altered states
            //
            //
            //------------------------------------------------------------------
            size_t probe = 0;
        PROBE:
            ++probe;
            Y_XMLOG(xml, "[[ #probe = " << probe << " ]]");
            const size_t nbad = unbalanced(C, L, xml);
            Y_XMLOG(xml, "#invalid = " << nbad);
            Y_XMLOG(xml, "#altered = " << altered.size());

            if(altered.size()<=0) return nbad;

            //------------------------------------------------------------------
            //
            //
            // process altered states
            //
            //
            //------------------------------------------------------------------
            {
                assert(altered.size()>=1);
                HeapSort::Call(altered, Altered::Compare);
                displayAltered(xml,"Initial");
                prune();
                displayAltered(xml,"Pruned");
                assert(altered.size()>=1);
            }

            switch(altered.size())
            {
                case 0: throw Specific:: Exception(CallSign,"corrupted #altered");
                case 1:
                    rcl.transfer(C, L, altered[1].cc, SubLevel);
                    goto PROBE;

                default:
                    break;
            }

            updateWithSwell(C, L, xml);
            goto PROBE;

        }

        void Equalizer:: updateWithSwell(XWritable &C, const Level L, XMLog & )
        {
            const size_t       nalt       = altered.size(); assert(nalt>=2);

            //------------------------------------------------------------------
            //
            //
            // initialize swell
            //
            //
            //------------------------------------------------------------------
            for(const SNode *sn=rcl.unbounded.list.head;sn;sn=sn->next)
            {
                const Species &sp = **sn;
                XAdd          &sw = swell[ sp.indx[SubLevel] ];
                sw.free();
                sw << C[ sp.indx[L] ];
            }

            for(const SNode *sn=rcl.conserved.list.head;sn;sn=sn->next)
                swell[ (**sn).indx[ SubLevel] ].free();

            //------------------------------------------------------------------
            //
            //
            // build swell
            //
            //
            //------------------------------------------------------------------
            const AddressBook &unbounded = rcl.unbounded.book;
            for(size_t i=nalt;i>0;--i)
            {
                const Altered     &alt = altered[i];
                const Equilibrium &eq  = alt.eq;
                const XReadable   &cc  = alt.cc;
                const XReadable   &dc  = alt.dc;

                std::cerr << "using " << eq << std::endl;
                {
                    size_t             nc = eq->size();
                    for(Components::ConstIterator it=eq->begin();nc>0;--nc,++it)
                    {
                        const Component &cm = **it;
                        const Species   &sp = cm.sp;
                        const size_t     jj = sp.indx[SubLevel];
                        XAdd            &sw = swell[jj];
                        if( unbounded.has(sp) )
                        {
                            sw << dc[jj]; // increase with extent
                        }
                        else
                        {
                            assert(sw.isEmpty());
                            sw <<  cc[jj]; // directly set
                        }
                    }
                }
            }

            //------------------------------------------------------------------
            //
            //
            // compute new state
            //
            //------------------------------------------------------------------
            for(const SNode *sn=rcl.species.head;sn;sn=sn->next)
            {
                const Species &      sp   = **sn;
                const size_t * const indx = sp.indx;
                XAdd         &       sw   = swell[ indx[SubLevel] ];
                //std::cerr << "d[" << sp << "] = " << sw << std::endl;
                C[ indx[L]  ] = sw.sum();
            }
        }

        void Equalizer:: displayAltered(XMLog &xml, const char * const title) const
        {
            if(!xml.verbose) return;
            Y_XML_SECTION(xml, title);
            const size_t nalt = altered.size();
            for(size_t i=1;i<=nalt;++i)
            {
                const Altered &alt = altered[i];
                rcl.uuid.pad( xml() << alt.eq, alt.eq);
                *xml << ": gain=" << std::setw(15) << real_t(alt.gg);
                alt.eq.displayCompact(*xml << " @", alt.cc, SubLevel);
                *xml << std::endl;
            }
        }


        bool Equalizer:: hasBestEffort(const Boundary   &limiting,
                                       const Boundaries &required)
        {
            best.empty();
            assert(limiting.size>0); assert(limiting.xi>=0.0);
            assert(required.size>0);

            const xreal_t    ximax = limiting.xi;
            const Boundary  *lower = 0;

            for(const BNode *node  = required.head;node;node=node->next)
            {
                const Boundary &here = **node;
                switch(Sign::Of(here.xi,ximax))
                {
                    case Negative:
                        lower = &here; // record for later
                        continue;

                    case __Zero__:
                        best.xi = ximax;     // numerical match
                        best   << here;      // load vanishing in here
                        best   << limiting;  // load vanishing in limiting
                        return true;

                    case Positive:
                        goto DONE; // no more possibility

                }
            }

        DONE:
            if(0 != lower)
            {
                //--------------------------------------------------------------
                //
                // found a lower (than ximax) boundary
                //
                //--------------------------------------------------------------
                const Boundary &here = *lower;
                best.xi = here.xi;
                best   << here;
                return true;
            }
            else
            {
                if(ximax>0.0)
                {
                    //----------------------------------------------------------
                    //
                    // best effort to reduce negative side
                    //
                    //----------------------------------------------------------
                    best.xi = ximax;
                    best << limiting;
                    return true;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // nothing to do
                    //
                    //----------------------------------------------------------
                    return false;
                }
            }
        }

        void Equalizer:: addBestAlter(const Equilibrium &eq,
                                      XWritable         &cc,
                                      XWritable         &dc)
        {
            const xreal_t              zero;
            const xreal_t              xi = best.xi; assert( xi.abs() > zero );
            Equilibrium::ConstIterator it = eq->begin();

            //------------------------------------------------------------------
            //
            // compute new concentration whilst gathering gain
            //
            //------------------------------------------------------------------
            xadd.free();
            for(size_t j=eq->size();j>0;--j,++it)
            {
                const Component &cm   = **it;
                const Species   &sp   = cm.sp;
                const xreal_t    dd   = xi * cm.xn;
                const size_t     jj   = sp.indx[SubLevel];
                const xreal_t    cOld = cc[jj];
                const xreal_t    cNew = (cc[jj] = cOld + dd);
                if(cOld<zero) xadd << -cOld;
                if(cNew<zero) xadd <<  cNew;
                dc[jj] = dd;
            }

            //------------------------------------------------------------------
            //
            // ensure strictly vanishing species
            //
            //------------------------------------------------------------------
            for(const SNode *node=best.head;node;node=node->next)
            {
                const Species &sp = **node;
                cc[ sp.indx[SubLevel] ] = zero;
            }

            //------------------------------------------------------------------
            //
            // add new state with computed gain
            //
            //------------------------------------------------------------------
            altered << Altered(eq,xi,cc,xadd.sum(),dc);
        }


        void Equalizer:: prune() noexcept
        {
            for(size_t i=2;i<=altered.size();)
            {
                const Equilibrium    &lhs  = altered[i].eq;
                const size_t          lid  = lhs.indx[ SubLevel ];
                const Readable<bool> &bit  = rcl.detached[lid];
                bool                  keep = true;
                for(size_t j=1;j<i;++j)
                {
                    const Equilibrium &rhs  = altered[j].eq;
                    const size_t       rid  = rhs.indx[ SubLevel ];
                    const bool         flag = bit[rid];
                    if(flag)
                    {
                        continue;
                    }
                    else
                    {
                        keep = false;
                        break;
                    }
                }

                if(!keep)
                    altered.remove(i); // remove and keep index
                else
                    ++i;               // keep and forward index

            }
        }




    }

}
