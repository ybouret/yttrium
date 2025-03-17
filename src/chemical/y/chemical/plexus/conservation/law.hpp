//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/type/defs.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            class Rule;

            //! law and its data to solve
            class Law :  public Actors
            {
            public:
                explicit Law(Rule * const); //!< setup and latch
                virtual ~Law() noexcept;
                
                const unsigned norm2;
                Law *          next;
                Law *          prev;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
            };

        }
    }
}
#endif
