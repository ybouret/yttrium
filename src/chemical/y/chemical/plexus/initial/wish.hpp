
//! \file

#ifndef Y_Plexus_Initial_Wish_Included
#define Y_Plexus_Initial_Wish_Included 1

#include "y/chemical/species.hpp"
#include "y/chemical/type/defs.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Player : public Object
        {
        public:
            explicit Player(const int      n,
                            const Species &s) noexcept;
            virtual ~Player() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Player);
        };

        class Wish : public Quantized, public Entity
        {
        public:
            static const char * const CallSign;
            
            explicit Wish();
            virtual ~Wish() noexcept;


            const xreal_t target;
            Wish * next;
            Wish * prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wish);
        };
    }
}

#endif

