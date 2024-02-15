//! \file

#ifndef Y_MKL_TAO_XAddPtr_Included
#define Y_MKL_TAO_XAddPtr_Included 1

#include "y/mkl/antelope/add.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {
            //__________________________________________________________________
            //
            //
            //
            //! helper to store temporary XAdd pointer
            /**
             as we don't know the type that will be used,
             we attach the specific XAdd before the computation
             */
            //
            //
            //__________________________________________________________________
            class XAddPtr
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit XAddPtr() noexcept; //!< setup
                virtual ~XAddPtr() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! reset
                void detach() noexcept;

                //! attach a XAdd
                template <typename U> inline
                void attach( Antelope::Add<U> &host ) noexcept
                {
                    handle = &host;
                }

                //! cast internal address
                template <typename U> inline
                Antelope::Add<U> & xadd() noexcept
                {
                    assert(0!=handle);
                    return * static_cast< Antelope::Add<U> * >(handle);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(XAddPtr);
                void *handle;
            };
        }
    }
}

#endif

