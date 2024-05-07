
//! \file

#ifndef Y_Ink_Edges_Included
#define Y_Ink_Edges_Included 1

#include "y/ink/ops/edge.hpp"
#include "y/data/small/heavy/list/bare.hpp"
#include "y/ink/pixmap.hpp"

namespace Yttrium
{
    namespace Ink
    {

        class Edges : public Proxy<const Edge::List>
        {
        public:
            typedef Small::BareHeavyList<Coord> CoordStore;
            typedef Pixmap<size_t>              Labels;

            explicit Edges() noexcept;
            virtual ~Edges() noexcept;

           
            void operator()(Slabs                    &slabs,
                            Labels                   &labels,
                            const Pixmap<uint8_t>    &force,
                            const Edge::Connectivity  conn);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            virtual ConstInterface & surrogate() const noexcept { return edges; }
            Edge::List edges;
            CoordBank  cbank;
            static Edge *RemoveEdge(Edge *, Labels &) noexcept;
        };

       

    }

}

#endif

