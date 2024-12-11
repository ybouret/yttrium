
//! \file

#ifndef Y_Chemical_Mixes_Included
#define Y_Chemical_Mixes_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/reactive/equilibria.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Mixes : public Proxy<const Mix::List>
        {
        public:
            static const char * const CallSign;
            explicit Mixes(Equilibria &eqs,
                           XMLog  &    xml);
            virtual ~Mixes() noexcept;
            Y_OSTREAM_PROTO(Mixes);;

        private:
            Mix::List my;

            Y_DISABLE_COPY_AND_ASSIGN(Mixes);
            virtual ConstInterface & surrogate() const noexcept;
            void                     checkFusion();
        };

    }

}

#endif

