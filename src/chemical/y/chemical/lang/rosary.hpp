
//! \file

#ifndef Y_Chemical_Rosary_Included
#define Y_Chemical_Rosary_Included 1

#include "y/chemical/species/library.hpp"
#include "y/singleton.hpp"
#include "y/jive/module.hpp"

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
            class Compiler;

            void operator()(Jive::Module *m,
                            Library      &lib);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Rosary);
            friend class Singleton<Rosary>;

            explicit Rosary();
            virtual ~Rosary() noexcept;
        };


    }

}

#endif
