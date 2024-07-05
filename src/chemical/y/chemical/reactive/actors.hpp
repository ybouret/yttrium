
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Actors : public Entity, public Proxy<const Actor::List>
        {
        public:
            explicit Actors() noexcept;
            virtual ~Actors() noexcept;
            Actors(const Actors &);
            Y_OSTREAM_PROTO(Actors);

            void add(Actor *a);
            void add(const unsigned nu, const Species &sp);

        private:
            Y_DISABLE_ASSIGN(Actors);
            Actor::List actors;
            virtual ConstInterface & surrogate() const noexcept;
            void growName(const String &);

        };

    }

}

#endif

