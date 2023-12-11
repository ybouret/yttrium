


//! \file

#ifndef Y_Concurrent_Resource1D_Included
#define Y_Concurrent_Resource1D_Included 1

#include "y/concurrent/resource/interface.hpp"
#include "y/concurrent/split/for-loop.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! 1D Resource for Loop
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Resource1D : public Resource< ForLoop<T> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef Resource< ForLoop<T> >     BaseType; //!< alias
            typedef typename BaseType::Mapping Mapping;  //!< alias
            typedef typename Mapping::Type     Type;     //!< alias
            typedef typename Mapping::Size     Size;     //!< alias
            typedef typename Mapping::Locus    Locus;    //!< alias


            //! init: compute ForLoop then activate
            inline void init(const T head,
                             const T tail,
                             const T step)
            {
                Mapping temp = Split::For(*this,head,tail,step);
                this->xch(temp);
                this->activate();
            }

            //! quit: erase ForLoop and shutdown
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
            inline virtual ~Resource1D() noexcept {} //!< cleanup
        protected:
            inline explicit Resource1D(const ThreadContext &ctx) noexcept : BaseType(ctx) {} //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource1D);
        };

    }

}

#endif
