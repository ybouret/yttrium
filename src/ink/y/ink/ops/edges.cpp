
#include "y/ink/ops/edges.hpp"
#include "y/sort/merge.hpp"

namespace Yttrium
{
    namespace Ink
    {



        Edges::  Edges() noexcept : Proxy<const Edge::List>(), edges(), cbank() {}
        Edges:: ~Edges() noexcept {}

        static inline void ZeroLabel(Slab &slab, Edges::Labels &label)
        {
            static const size_t zero = 0;
            slab.load(label,zero);
        }

        static inline SignType ByDecreasingSize(const Edge * const lhs,
                                                const Edge * const rhs) noexcept
        {
            return Sign::Of(rhs->size,lhs->size);
        }


        Edge * Edges:: RemoveEdge(Edge * const edge, Labels &label) noexcept
        {
            assert(0!=edge);
            for(const CoordNode *node = edge->head; node; node=node->next)
            {
                const Coord pos = **node;
                label[pos] = 0;
            }
            return edge;
        }

        void Edges:: operator()(Slabs                    &slabs,
                                Labels                   &label,
                                Pixels                   &force,
                                const Edge::Connectivity conn)
        {
            //__________________________________________________________________
            //
            //
            // initializing
            //
            //__________________________________________________________________
            edges.release();
            slabs(ZeroLabel,label);
            //cbank->ensure(force.n);

            //__________________________________________________________________
            //
            //
            // loop over each pixel
            //
            //__________________________________________________________________
            const unit_t h = force.h;
            const unit_t w = force.w;
            for(unit_t j=0;j<h;++j)
            {
                //______________________________________________________________
                //
                //
                // loop over each row
                //
                //______________________________________________________________
                PixRow<size_t>        &label_j = label[j];
                const PixRow<uint8_t> &force_j = force[j];
                for(unit_t i=0;i<w;++i)
                {
                    //__________________________________________________________
                    //
                    //
                    // probe current pixel status
                    //
                    //__________________________________________________________
                    size_t &      label_ji = label_j[i]; if(label_ji>0)    { assert(label_ji<=edges.size); continue; } // in another edge
                    const uint8_t force_ij = force_j[i]; if(force_j[i]<=0) continue;                                   // nothing here

                    //__________________________________________________________
                    //
                    //
                    // Create a new edge
                    //
                    //__________________________________________________________
                    Edge         *edge = edges.pushTail( new Edge( edges.size+1, cbank) );
                    CoordList     scan(cbank);
                    const size_t  indx = edge->label;
                    bool          ripe = false;
                    //__________________________________________________________
                    //
                    //
                    // initialize search
                    //
                    //__________________________________________________________
                    scan     << Coord(i,j);
                    label_ji = indx;
                    ripe     = (force_ij >= 255);

                    //__________________________________________________________
                    //
                    //
                    // search
                    //
                    //__________________________________________________________
                    while(scan.size>0)
                    {
                        const size_t n = scan.size;
                        for(size_t k=n;k>0;--k)
                        {
                            CoordNode  *node = edge->pushTail(scan.popTail());
                            const Coord here = **node; assert(indx==label[here]);
                            for(unsigned p=0;p<conn;++p)
                            {
                                const Coord pos = here + Edge::Delta[p];
                                if(!label.contains(pos))                     continue;    // out of domain
                                if(label[pos]>0) { assert(indx==label[pos]); continue; }  // already visited
                                const uint8_t f = force[pos]; if(f<=0)       continue;    // background
                                scan       << pos;
                                label[pos] = indx;
                                if(f>=255) ripe = true;
                            }
                        }
                    }
                    //std::cerr << "#edge=" << edge->size << " / ripe=" << ripe << std::endl;
                    if(!ripe)
                    {
                        delete RemoveEdge( edges.popTail(), label);
                    }
                }
            }
            //std::cerr << "#edges=" << edges.size << std::endl;

            //__________________________________________________________________
            //
            //
            // sorting edges
            //
            //__________________________________________________________________
            MergeSort::Call(edges,ByDecreasingSize);
            //while(edges.tail && edges.tail->size <= 1) delete RemoveEdge( edges.popTail(), label);
            std::cerr << "#edges=" << edges.size << std::endl;
            if(edges.size)
            {
                std::cerr << "\tfrom " << edges.head->size << " to " << edges.tail->size << std::endl;
            }
        }
    }

}


