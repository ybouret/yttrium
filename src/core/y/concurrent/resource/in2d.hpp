


//! \file

#ifndef Y_Concurrent_Resource2D_Included
#define Y_Concurrent_Resource2D_Included 1

#include "y/concurrent/resource/interface.hpp"
#include "y/concurrent/split/tiling.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        template <typename T>
        class Resource2D : public Resource< AutoPtr< typename Tiling<T>::Tile > >
        {
        public:
            typedef typename Tiling<T>::Tile     Tile;
            typedef Resource< AutoPtr<Tile> >    BaseType;
            typedef typename BaseType::Mapping   Mapping;
            typedef typename Tiling<T>::Type     Type;
            typedef typename Tiling<T>::Size     Size;
            typedef typename Tiling<T>::Locus    Locus;      //!< alias
            typedef typename Tiling<T>::Vertex   Vertex;     //!< alias
            typedef typename Tiling<T>::Segment  Segment;
            typedef typename Tiling<T>::Iterator Iterator;

            //! init: compute tile then  activate
            inline void init(const Vertex lower, const Vertex upper)
            {
                Mapping temp = Tiling<T>::Tiles::For(*this,lower,upper); // may be NULL
                this->xch(temp);
                this->activate();
            }

            inline void quit() noexcept
            {
                Mapping temp;
                this->xch(temp);
                this->shutdown();
            }


            inline virtual ~Resource2D() noexcept {}

        protected:
            inline explicit Resource2D(const ThreadContext &ctx) : BaseType(ctx) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource2D);
        };


    }

}

#endif
