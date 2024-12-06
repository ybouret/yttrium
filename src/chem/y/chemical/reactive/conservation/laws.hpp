
//! \file

#ifndef Y_Chemical_Conservation_Laws_Included
#define Y_Chemical_Conservation_Laws_Included 1

#include "y/chemical/reactive/conservation/canon.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Cluster;

        namespace Conservation
        {
            
            class Laws : public Object, public Proxy<const Canon>
            {
            public:
                static const char * const CallSign;
                virtual ~Laws() noexcept;
                explicit Laws(Cluster &, XMLog &);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Laws);
                Canon canon;
                virtual ConstInterface & surrogate() const noexcept;
            };


        }

    }


}

#endif

