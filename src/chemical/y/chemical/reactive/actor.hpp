//! \file

#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chemical/species.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Actor : public Entity
        {
        public:
            typedef CxxListOf<Actor> List;

            explicit Actor(const unsigned n, const Species &s);
            virtual ~Actor() noexcept;
            Actor(const Actor &);
            Y_OSTREAM_PROTO(Actor);
            const Actor & operator*() const noexcept;
            
            const unsigned  nu;
            const Species  &sp;
            const unsigned  n1;
            Actor *         next;
            Actor *         prev;
            
        private:
            Y_DISABLE_ASSIGN(Actor);
            void initialize();

        };

    }
}

#endif

