#include "y/chemical/plexus/conservation/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Warden:: Warden(const Cluster &_cluster,
                            const Canon   &_canon) :
            cluster(_cluster),
            canon(_canon),
            xadd(),
            blist(canon.size),
            cproj(canon.size+1,canon.species->size),
            c0(cproj[canon.size+1]),
            injected(canon.species->size),
            next(0),
            prev(0)
            {
            }

            Warden::  ~Warden() noexcept
            {
            }

            std::ostream & Warden:: display(std::ostream &os, const Broken &b) const
            {
                canon.pad(os << b.law.name,b.law) << " | " << std::setw(22) << b.xs.str();
                return os;
            }

            static inline SignType CompareBroken(const BNode * const lhs,
                                                 const BNode * const rhs) noexcept
            {
                return Sign::Of( (**lhs).xs, (**rhs).xs);
            }

            void Warden:: operator()(XMLog     & xml,
                                     XWritable & C0,
                                     const Level L0)
            {
                Y_XML_SECTION_OPT(xml, "Warden", "|canon|=" << canon.size);
                const xreal_t zero;

                injected.forEach(& XAdd::free );
                {
                    Y_XML_SECTION(xml,"Initialize");
                    //----------------------------------------------------------
                    //
                    //
                    // populate list of broken laws
                    //
                    //
                    //----------------------------------------------------------
                    blist.free();
                    canon.transfer(c0,AuxLevel,C0,L0);

                    for(const LNode *ln=canon.head;ln;ln=ln->next)
                    {
                        const Law &   law = **ln;
                        const xreal_t xs  = law.excess(xadd,c0,AuxLevel);
                        if(xs>zero)
                        {
                            XWritable &cc = cproj[blist.size+1].ld(c0);
                            law.project(xadd,cc,c0,AuxLevel);
                            const Broken broken(law,xs,cc);
                            blist << broken;
                            if(xml.verbose) display(   xml() << "[broken] ", broken) << std::endl;
                        }
                        else {
                            if(xml.verbose) canon.pad( xml() << "[ -ok- ] " << law.name,law) << std::endl;
                        }
                    }
                }


                {
                    //----------------------------------------------------------
                    //
                    //
                    // reduce list of broken laws by iterative mending
                    //
                    //
                    //----------------------------------------------------------
                    size_t iter = 0;
                    while(blist.size>0)
                    {
                        Y_XML_SECTION_OPT(xml, "MendBroken", "iteration=" << ++iter);

                        //------------------------------------------------------
                        //
                        // find the smallest increase
                        //
                        //------------------------------------------------------
                        MergeSort::Call(blist,CompareBroken);
                        {
                            Broken &best = **blist.head;
                            Y_XML_COMMENT(xml,"best: " << best.law.name);
                            const Law &law = best.law;

                            // set c0 to new value
                            c0.ld(best.cc);

                            // update injected
                            for(const Actor *a=law->head;a;a=a->next)
                            {
                                const xreal_t delta = (a->xn * best.xs) / law.denom;
                                a->sp(injected,AuxLevel) << delta;
                            }
                        }

                        //------------------------------------------------------
                        //
                        // keep still broken laws
                        //
                        //------------------------------------------------------
                        blist.cutHead(); // discard
                        for(BNode *node=blist.head;node;)
                        {
                            BNode * const next = node->next;
                            Broken &      curr = **node;
                            curr.xs = curr.law.excess(xadd,c0,AuxLevel);
                            if(curr.xs<=zero)
                            {
                                // drop
                                Y_XML_COMMENT(xml, "drop: " << curr.law.name);
                                blist.cutNode(node);
                                node=0;
                            }
                            else
                            {
                                // keep with RELOADING before projection
                                Y_XML_COMMENT(xml, "keep: " << curr.law.name);
                                curr.law.project(xadd, curr.cc.ld(c0), c0, AuxLevel );
                            }
                            node=next;
                        }

                        if(xml.verbose)
                        {
                            for(const BNode *node=blist.head;node;node=node->next)
                            {
                                display(   xml() << "[broken] ", **node) << std::endl;
                            }
                        }
                    }
                }

                if(xml.verbose)
                {
                    canon.show( xml() << "injected=",injected);
                }

                canon.transfer(C0,L0,c0,AuxLevel);

            }

        }

    }
}

