


//! \file

#ifndef Y_Concurrent_Resource2D_Included
#define Y_Concurrent_Resource2D_Included 1

#include "y/concurrent/resource/interface.hpp"
#include "y/concurrent/split/tiling.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! 2D Resource for Loop
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Resource2D : public Resource< AutoPtr< typename Tiling<T>::Tile > >
        {
        public: 
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef typename Tiling<T>::Tile     Tile;      //!< alias
            typedef Resource< AutoPtr<Tile> >    BaseType;  //!< alias
            typedef typename BaseType::Mapping   Mapping;   //!< alias
            typedef typename Tiling<T>::Type     Type;      //!< alias
            typedef typename Tiling<T>::Size     Size;      //!< alias
            typedef typename Tiling<T>::Locus    Locus;     //!< alias
            typedef typename Tiling<T>::Vertex   Vertex;    //!< alias
            typedef typename Tiling<T>::Segment  Segment;   //!< alias
            typedef typename Tiling<T>::Iterator Iterator;  //!< alias

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! init: compute tile then  activate
            inline void init(const Vertex lower, const Vertex upper)
            {
                Mapping temp = Tiling<T>::Tiles::For(*this,lower,upper); // may be NULL
                this->xch(temp);
                this->activate();
            }

            //! quit: erase tile then shutdown
            inline void quit() noexcept
            {
                Mapping temp;
                this->xch(temp);
                this->shutdown();
            }


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline virtual ~Resource2D() noexcept {} //!< cleanup

        protected:
            //! setup
            inline explicit Resource2D(const ThreadContext &ctx) noexcept : BaseType(ctx) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource2D);
        };


    }

}

#endif
