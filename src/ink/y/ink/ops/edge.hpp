
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

        //______________________________________________________________________
        //
        //
        // Aliases
        //
        //______________________________________________________________________
        typedef Small::CoopHeavyList<Coord> CoordList; //!< coop list of coordinates
        typedef CoordList::NodeType         CoordNode; //!< alias
        typedef CoordList::ProxyType        CoordBank; //!< shared bank of coordinate

        //______________________________________________________________________
        //
        //
        //
        //! Edge is a list of coordinates
        //
        //
        //______________________________________________________________________
        class Edge : public Object, public CoordList
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef CxxListOf<Edge> List; //!< alias

            //! building connectivity
            enum Connectivity
            {
                Connect4 = 4, //!< four neighbours
                Connect8 = 8  //!< eight neighbours
            };
            static const Coord Delta[8]; //!< directions lookup

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Edge(const size_t, const CoordBank &); //!< setup with label and bank
            virtual ~Edge() noexcept;                       //!< cleanup
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t label; //!< label>0
            Edge        *next;  //!< for list
            Edge        *prev;  //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edge);
        };

    }

}

#endif

