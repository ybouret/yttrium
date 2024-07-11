
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/plexus/cluster/conservation/law.hpp"
#include "y/chemical/assembly.hpp"
#include "y/container/matrix.hpp"
#include "y/stream/xmlog.hpp"
#include "y/container/cxx/array.hpp"

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
            //! List of Laws
            //
            //
            //__________________________________________________________________
            class Laws : public Law::List, public Assembly
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
               
                //! setup
                explicit Laws(const SList            &sp,
                              const Matrix<unsigned> &Qm);

                //! cleanup
                virtual ~Laws() noexcept;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
            };
        }

    }

}



#endif

