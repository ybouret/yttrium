
//! \file

#ifndef Y_Concurrent_Engine1D_Included
#define Y_Concurrent_Engine1D_Included 1

#include "y/concurrent/engine/interface.hpp"
#include "y/concurrent/split/for-loop.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //
        //! 1D engine: based of ForLoop<T>
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Engine1D : public Engine< ForLoop<T> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ForLoop<T>              Mapping;    //!< alias
            typedef Engine<Mapping>         EngineType; //!< alias
            typedef T                       Type;       //!< alias
            typedef typename Mapping::Size  Size;       //!< alias
            typedef typename Mapping::Locus Locus;      //!< alias

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! start engine: compute loop then initiate/activate
            inline void start(const ThreadContext &cntx,
                              const T head,
                              const T tail,
                              const T step)
            {
                Mapping temp = Split::For(cntx,head,tail,step);
                this->initiate(cntx,temp);
            }

         


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline virtual ~Engine1D() noexcept {}
        protected:
            inline explicit Engine1D() noexcept : EngineType() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine1D);
        };
    }

}

#endif

