//! \file

#ifndef Y_CONCURRENT_FRAME1D_INCLUDED
#define Y_CONCURRENT_FRAME1D_INCLUDED 1

#include "y/concurrent/split/trek.hpp"
#include "y/concurrent/frame/interface.hpp"
#include "y/ptr/bare.hpp"

namespace Yttrium
{
    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! Frame for 1D operations
        //
        //
        //______________________________________________________________________
        template <typename T>
        class Frame1D : public Frame< Trek<T> >
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef T                      Type;       //!< alias
            typedef Trek<T>                Mapping;    //!< alias
            typedef Frame<Mapping>         FrameType;  //!< alias
            typedef BarePtr<const Mapping> TrekPtr;    //!< alias

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! assign partial ForLoop
            /**
             - compute partial ForLoop for context
             - assign loop if resulting length is positive
             */
            inline void assign(const T &head, const T &tail, const T &step)
            {
                const Mapping &here = divided(head,tail,step);
                if(here.length>0) Coerce(trek) = &here;
            }

            //! access sub ForLoop
            inline const Trek<T> * operator->() const noexcept
            {
                assert(this->isAssigned());
                return & *(this->workspace);
            }

            //! access sub ForLoop
            inline const Trek<T> & operator*() const noexcept
            {
                assert(this->isAssigned());
                return  *(this->workspace);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            
            //! partial ACTIVE trek
            const Trek<T> * const trek;


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! cleanup
            inline virtual ~Frame1D() noexcept {}

        protected:
            //! setup
            inline explicit Frame1D(const ThreadContext &ctx) noexcept :
            FrameType(ctx), trek(0)
            {
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Frame1D);

            inline const Mapping & divided(const T &head, const T &tail, const T &step) noexcept
            {
                assert(!this->isAssigned());
                assert(0==trek);
                const Mapping  part = Split::For(*this, head, tail, step);
                return this->workspace.build(part);
            }

            //! call in loosen
            inline virtual void shutdown() noexcept { Coerce(trek) = 0; }
        };
    }

}

#endif

