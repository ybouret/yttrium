//! \file

#ifndef Y_Type_AutoClean_Included
#define Y_Type_AutoClean_Included 1

#include "y/config/starting.hpp"
#include "y/check/static.hpp"
#include <iostream>

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! calling method of an host before return
    /**
     simplify code in case of exception
	 The size of a pointer-to-member-function can change depending on the class!
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
            template <typename HOST>
            inline  Args(HOST &userHost, void (HOST::*hostMeth)(void) )  noexcept :
            host( &userHost ), meth( 0 )
            {
                typedef void (HOST::*METH)(void);

				void (HOST::*hm)(void) = 0;
				void (AutoClean::*am)(void) = 0;

				std::cerr << "sizeof(hm)=" << sizeof(hm) << std::endl;
				std::cerr << "sizeof(am)=" << sizeof(am) << std::endl;

               // Y_STATIC_CHECK(sizeof(METH)==sizeof(Meth),BadMethodSize);
                // transmogrify method
                union
                {
                    METH user;
                    Meth mine;
                } alias = { hostMeth };
                meth = alias.mine;


                std::cerr << "alias.user=" << alias.user << "/" << meth << " sizeof=" << sizeof(Meth) << std::endl;
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

