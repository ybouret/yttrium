
//! \file

#ifndef Y_Chemical_Rosary_Included
#define Y_Chemical_Rosary_Included 1

#include "y/chemical/species/library.hpp"
#include "y/singleton.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! Rapid Obnoxious Species And Reactions Yoga
        class Rosary : public Singleton<Rosary>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = 0;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Rosary);
            friend class Singleton<Rosary>;
            class Compiler;
            
            explicit Rosary();
            virtual ~Rosary() noexcept;
        };


    }

}

#endif
