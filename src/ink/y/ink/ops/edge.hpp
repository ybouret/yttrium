
//! \file

#ifndef Y_Ink_Edge_Included
#define Y_Ink_Edge_Included 1

#include "y/ink/coord.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Ink
    {

        typedef Small::CoopHeavyList<Coord> CoordList;
        typedef CoordList::NodeType         CoordNode;
        typedef CoordList::ProxyType        CoordBank;

        class Edge : public Object, public CoordList
        {
        public:
            typedef CxxListOf<Edge> List;

            enum Connectivity
            {
                Connect4 = 4,
                Connect8 = 8
            };
            static const Coord Delta[8];

            
            explicit Edge(const size_t, const CoordBank &);
            virtual ~Edge() noexcept;
            

            const size_t label;
            Edge        *next;
            Edge        *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edge);
        };

    }

}

#endif

