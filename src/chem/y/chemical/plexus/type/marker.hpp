

//! \file

#ifndef Y_Chemical_Marker_Included
#define Y_Chemical_Marker_Included 1

#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Marker : public Proxy<const SRepo>
        {
        public:
            explicit Marker(const SBank & _sbank) noexcept;
            Marker(const Marker &);
            virtual ~Marker() noexcept;
            Y_OSTREAM_PROTO(Marker);    //!< dispay


        protected:
            SRepo srepo;
            xReal value;
            
        private:
            Y_DISABLE_ASSIGN(Marker);
            Y_PROXY_DECL();

        };
    }

}

#endif
