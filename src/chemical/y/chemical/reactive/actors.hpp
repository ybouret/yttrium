
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Actors : public CxxListOf<Actor>
        {
        public:
            explicit Actors() noexcept;
            virtual ~Actors() noexcept;
            Actors(const Actors &);

            String toString() const;

            friend std::ostream & operator<<(std::ostream &os, const Actors &A);
            

        private:
            Y_DISABLE_ASSIGN(Actors);
        };

    }

}

#endif
