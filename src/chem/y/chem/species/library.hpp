
//! \file


#ifndef Y_Chemical_Library_Included
#define Y_Chemical_Library_Included 1

#include "y/chem/type/entity.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Library of species
        //
        //
        //______________________________________________________________________
        class Library
        {
        public:
            explicit Library();
            virtual ~Library() noexcept;

        private:
            Y_DISABLE_ASSIGN(Library);
        };

    }

}

#endif

