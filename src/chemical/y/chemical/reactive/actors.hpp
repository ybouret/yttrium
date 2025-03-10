

//! \file


#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1


#include "y/chemical/reactive/actor.hpp"
#include "y/chemical/type/latchable.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Actors : public Entity, public Proxy<const Actor::List>, public Latchable
        {
        public:
            static const char * const CallSign; //!< "Actors"

            explicit Actors(const Actor::Involvement); //!< setup
            Actors(const Actors &);                    //!< duplicate all
            virtual ~Actors() noexcept;                //!< cleanup

            const Actor & operator()(const unsigned nu, const Species &sp);
            bool has(const Species &) const noexcept;
            void xch(Actors &) noexcept;

            const Actor::Involvement in; //!< involvement

        private:
            Y_DISABLE_ASSIGN(Actors);
            Y_PROXY_DECL();
            Actor::List my; //!< my list

        };
    }

}

#endif
