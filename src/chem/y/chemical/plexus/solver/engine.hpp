
//! \file

#ifndef Y_Chemical_Engine_Included
#define Y_Chemical_Engine_Included 1

#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Engine : public ProBank, public ProList, public Assembly
        {
        public:
            explicit Engine();
            virtual ~Engine() noexcept;

            void update() noexcept;
            void reset()  noexcept;

            void  show(XMLog &xml) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
        };

    }

}

#endif

