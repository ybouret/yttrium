
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
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit Logic(const uint32_t t) noexcept; //!< setup
            Logic(const Logic &);                      //!< copy


        public:
            virtual ~Logic() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t serialize(OutputStream &fp) const;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Logic & operator<<(Pattern * const) noexcept;
            Logic & add(const uint8_t single);
            Logic & add(const uint8_t lower, const uint8_t upper);

        private:
            Y_DISABLE_ASSIGN(Logic);
        };

    }

}

#endif

