
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            //__________________________________________________________________
            //
            //
            //
            //! List of laws from conservation matrix
            //
            //
            //__________________________________________________________________
            class Laws : public Proxy< const ListOf<Law> >, public Assembly
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Laws(Rule *);     //!< setup from head node of Rules
                virtual ~Laws() noexcept;  //!< cleanup
                Y_OSTREAM_PROTO(Laws);     //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void xch( Laws &other ) noexcept; //!< exchagne laws and assembly


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                Y_PROXY_DECL();
                CxxListOf<Law> my;

            public:
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const SList species; //!< list of bounded species
            };

        }
    }
}


#endif
