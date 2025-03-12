

//! \file


#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1


#include "y/chemical/reactive/actor.hpp"
#include "y/chemical/type/latchable.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of actors
        //
        //
        //______________________________________________________________________
        class Actors : public Entity, public Proxy<const Actor::List>, public Latchable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Actors"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors(const Actor::Involvement); //!< setup
            virtual ~Actors() noexcept;                //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & operator()(const unsigned nu, const Species &sp); //!< create new actor
            bool          has(const Species &) const noexcept; //!< check presence

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actor::Involvement in; //!< involvement

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actors);
            Y_PROXY_DECL();
            Actor::List my; //!< my list

        };
    }

}

#endif
