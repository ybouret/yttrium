
//! \file

#ifndef Y_Lingo_Logic_Included
#define Y_Lingo_Logic_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Logic is a Pattern and a list of operands
        //
        //
        //______________________________________________________________________
        class Logic : public Pattern, public Pattern::List
        {
            
        protected:
            explicit Logic(const uint32_t t) noexcept; //!< setup
            Logic(const Logic &);                      //!< copy

        public:
            virtual ~Logic() noexcept;

            virtual size_t serialize(OutputStream &fp) const;
            

        private:
            Y_DISABLE_ASSIGN(Logic);
        };

    }

}

#endif

