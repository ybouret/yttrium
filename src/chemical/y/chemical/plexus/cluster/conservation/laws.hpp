
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/plexus/cluster/conservation/law.hpp"
#include "y/chemical/assembly.hpp"
#include "y/container/matrix.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            
            class Laws : public Law::List, public Assembly
            {
            public:
                virtual ~Laws() noexcept;
                explicit Laws(const SList            &sp,
                              const Matrix<unsigned> &Qm);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
            };
        }

    }

}



#endif

