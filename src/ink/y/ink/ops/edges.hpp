
//! \file

#ifndef Y_Ink_Edges_Included
#define Y_Ink_Edges_Included 1

#include "y/ink/ops/edge.hpp"
#include "y/ink/pixmap.hpp"

namespace Yttrium
{
    namespace Ink
    {

        //______________________________________________________________________
        //
        //
        //
        //! Edges is a list of edges
        //
        //
        //______________________________________________________________________
        class Edges : public Proxy<const Edge::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Pixmap<size_t>              Labels;     //!< alias
            typedef Pixmap<uint8_t>             Pixels;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Edges() noexcept; //!< setup
            virtual ~Edges() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build edges
            /**
             \param slabs  slabs to operate
             \param labels target labels
             \param force  input/fortified output
             \param conn   connectivity
             */
            void operator()(Slabs                    &slabs,
                            Labels                   &labels,
                            Pixels                   &force,
                            const Edge::Connectivity  conn);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            virtual ConstInterface & surrogate() const noexcept { return edges; }
            Edge::List edges;
            CoordBank  cbank;
            static Edge *RemoveEdge(Edge *, Labels &, Pixels &) noexcept;
        };

       

    }

}

#endif

