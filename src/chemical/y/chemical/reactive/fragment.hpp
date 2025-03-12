
//! \file


#ifndef Y_Chemical_Fragment_Included
#define Y_Chemical_Fragment_Included 1

#include "y/chemical/reactive/components.hpp"
#include "y/chemical/type/assembly.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        class Fragment  
        {
        public:
            explicit Fragment()        noexcept;
            virtual ~Fragment()        noexcept;
            Fragment(const Fragment &) noexcept;
            Fragment & operator=(const Fragment &) noexcept;

            void           enroll(const Components &eq) noexcept;
            std::ostream & display(std::ostream &, const Components &) const;

            Assembly self; //!< equilibria name
            Assembly reac; //!< reactants  name
            Assembly prod; //!< products   name

        };

    }

}


#endif

