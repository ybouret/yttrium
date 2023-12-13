//! \file

#ifndef Y_Type_AutoClean_Included
#define Y_Type_AutoClean_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! calling method of an host before return
    /**
     simplify code in case of exception
     */
    //
    //
    //__________________________________________________________________________
    class AutoClean
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef void (AutoClean::*Meth)(void); //!< alias

        //______________________________________________________________________
        //
        //
        //! Holding anonymous arguments
        //
        //______________________________________________________________________
        class Args
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! convert types to anonymous
            template <typename HOST, typename METH>
            inline  Args(HOST &userHost, METH hostMeth)  noexcept :
            host( &userHost ), meth( (Meth)hostMeth )
            {
            }

            //! cleanup
            ~Args() noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            void * const host; //!< host address
            Meth         meth; //!< method address

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Args);
        };

        typedef void (*Proc)(Args &); //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! gather host+method and select call
        template <typename HOST, typename METH> inline
        explicit AutoClean(HOST &host, METH meth) noexcept :
        args( host, meth), proc( Call<HOST,METH> )
        {
        }

        //! cleanup, safe call to host.method()
        virtual ~AutoClean() noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(AutoClean);
        Args args;
        Proc const proc;

        template <typename HOST, typename METH> static inline
        void Call(Args &args) noexcept
        {
            try {
                assert(0!=args.host);
                assert(0!=args.meth);
                HOST &host = *(HOST *) (args.host);
                METH  meth =  (METH  ) (args.meth);
                (host.*meth)();
            } catch(...) {}
        }

    };

}

#endif

