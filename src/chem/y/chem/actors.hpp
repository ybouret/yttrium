//! \file


#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chem/actor.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Actors : public CxxListOf<Actor>
        {
        public:
            typedef CxxListOf<Actor> BaseType;
            
            explicit Actors() noexcept;
            virtual ~Actors() noexcept;
            Actors(const Actors &);

            String toString() const;

        private:
            Y_DISABLE_ASSIGN(Actors);
        };

    }

}

#endif
