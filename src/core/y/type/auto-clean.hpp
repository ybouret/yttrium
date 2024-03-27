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
        typedef void (AutoClean::*Meth)(void); //!< alias to a method pointer

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
            host( &userHost ), meth( 0 )
            {
                // transmogrify method
                union
                {
                    METH user;
                    Meth mine;
                } alias = { hostMeth };
                meth = alias.mine;
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
        Args       args;
        Proc const proc;

        template <typename HOST, typename METH> static inline
        void Call(Args &args) noexcept
        {
            try {
                assert(0!=args.host);
                assert(0!=args.meth);
                HOST &host = *static_cast<HOST *>(args.host);
                union
                {
                    Meth mine;
                    METH meth;
                } alias = { args.meth };
                (host.*alias.meth)();
            } catch(...) {}
        }

    };

}

#endif

