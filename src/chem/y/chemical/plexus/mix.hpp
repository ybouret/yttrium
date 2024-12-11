//! \file

#ifndef Y_Chemical_Mix_Included
#define Y_Chemical_Mix_Included 1

#include "y/chemical/plexus/connected.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Mix : public Oversized, public Proxy<const Connected>
        {
        public:
            typedef CxxListOf<Mix> List;
            
            explicit Mix(const Equilibrium &first);
            virtual ~Mix() noexcept;
            Y_OSTREAM_PROTO(Mix);
            

            bool accepts(const Equilibrium &) const noexcept;
            bool accepts(const Mix &)         const noexcept;
            void addPrimary(const Equilibrium &);
            void addPrimary(const Mix &);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mix);
            virtual ConstInterface & surrogate() const noexcept;
            Connected my;

        public:
            Mix * next;
            Mix * prev;
        };

    }

}

#endif

