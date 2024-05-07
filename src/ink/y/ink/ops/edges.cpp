
#include "y/ink/ops/edges.hpp"

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

        void Edges:: build(Slabs                    &slabs,
                           Pixmap<size_t>           &label,
                           const Pixmap<uint8_t>    &force,
                           const Edge::Connectivity &conn)
        {
            //______________________________________________________________
            //
            //
            // initializing
            //
            //______________________________________________________________
            edges.release();
            slabs(ZeroLabel,label);
            //cbank->ensure(force.n);

            const unit_t h = force.h;
            const unit_t w = force.w;
            for(unit_t j=0;j<h;++j)
            {
                PixRow<size_t>        &label_j = label[j];
                const PixRow<uint8_t> &force_j = force[j];
                for(unit_t i=0;i<w;++i)
                {
                    size_t &label_ji = label_j[i];
                    if(label_ji>0)  continue; // in another edge
                    if(force_j[i]<=0) continue; // nothing here
                    Edge         *edge = edges.pushTail( new Edge( edges.size+1, cbank) );
                    CoordList     scan(cbank);
                    const size_t  indx = edge->label;
                    scan     << Coord(i,j);
                    label_ji = indx;

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
                                if(!label.contains(pos)) continue;
                                if(label[pos]>0) { assert(indx==label[pos]); continue; }
                                if(force[pos]<=0) continue;
                                scan       << pos;
                                label[pos] = indx;
                            }
                        }
                    }
                    std::cerr << "#edge=" << edge->size << std::endl;
                }
            }
            std::cerr << "#edges=" << edges.size << std::endl;
        }
    }

}


