
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
        altered(neq)
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
                    rcl.transfer(cc,SubLevel,C,L);

                    if(xml.verbose)
                    {
                        rcl.uuid.pad( xml() << Faders::TextFlag(id) << " | " << eq.name, eq) << std::endl;
                        if(id!=Faders::BALANCED)
                        {
                            xml() << "\treac: " << fd.reac << std::endl;
                            xml() << "\tprod: " << fd.prod << std::endl;
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
                                Y_XMLOG(xml, "\tbest: " << best);
                                addBestAlter(eq,cc);
                            }
                            else
                            {
                                Y_XMLOG(xml, "\tno best effort");
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
                                Y_XMLOG(xml, "\tbest: " << best);
                                addBestAlter(eq,cc);
                            }
                            else
                            {
                                Y_XMLOG(xml, "\tno best effort");
                            }
                        }  continue;
                    }

                    throw Specific:: Exception(CallSign,"corrupted unbalanced '%s'", eq.name.c_str());
                }
            }
            return invalid;
        }

        void Equalizer:: run(XWritable &C, const Level L, XMLog &xml)
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
            const size_t nbad = unbalanced(C, L, xml);
            Y_XMLOG(xml, "#invalid = " << nbad);
            Y_XMLOG(xml, "#altered = " << altered.size());

            if(altered.size()<=0) return;

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
                    break;

                default: {
                    const size_t nalt = altered.size(); assert(nalt>=2);
                    throw Specific::Exception(CallSign, "Not Implemented!");
                }
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
                                      XWritable         &cc)
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
                const xreal_t    dc   = xi * cm.xn;
                const size_t     jj   = sp.indx[SubLevel];
                const xreal_t    cOld = cc[jj];
                const xreal_t    cNew = (cc[jj] = cOld + dc);
                if(cOld<zero) xadd << -cOld;
                if(cNew<zero) xadd <<  cNew;
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
            altered << Altered(eq,cc,xadd.sum());
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
