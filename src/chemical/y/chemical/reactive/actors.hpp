
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of Actors
        //
        //
        //______________________________________________________________________
        class Actors : public Entity, public Proxy<const Actor::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors(const size_t level) noexcept; //!< setup with top-level index
            virtual ~Actors() noexcept;                   //!< cleanup
            Actors(const Actors &);                       //!< hard copy
            Y_OSTREAM_PROTO(Actors);                      //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void xch(Actors &other) noexcept;

            //! add next actor
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

