//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            class Law :  public  Actors
            {
            public:
                typedef CxxListOf<Law> List;

                explicit Law(const SList              &species,
                             const Readable<unsigned> &coef);
                virtual ~Law() noexcept;
                Y_OSTREAM_PROTO(Law);
                

            public:
                Law    *next;
                Law    *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                
            };
        }
    }
}


#endif

