
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
                void xch( Laws &other )          noexcept; //!< exchagne laws and assembly
                bool got(const Species &) const noexcept; //!< check among laws

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                Y_PROXY_DECL();
                CxxListOf<Law> my;
            };

        }
    }
}


#endif
