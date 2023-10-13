//! \file

#ifndef Y_Chemical_Conservation_Included
#define Y_Chemical_Conservation_Included 1

#include "y/chem/species/actors.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Conservation : public Object, public Proxy<const Actors>
        {
        public:
            typedef CxxListOf<Conservation> List;

            virtual ~Conservation() noexcept;
            explicit Conservation() noexcept;
            Y_OSTREAM_PROTO(Conservation);
            void add(const unsigned nu, const Species &sp);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Conservation);
            virtual ConstInterface & surrogate() const noexcept;
            Actors actors;

        public:
            Conservation *next;
            Conservation *prev;
        };

       

    }

}

#endif

