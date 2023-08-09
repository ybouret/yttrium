//! \file

#ifndef Y_Jive_Pattern_Included
#define Y_Jive_Pattern_Included 1

#include "y/jive/source.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Pattern recognition
        //
        //
        //______________________________________________________________________
        class Pattern : public Object, public Token
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual Pattern * clone() const = 0; //!< perform deep copy

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        public:  Pattern       *next; //!< for list
        public:  Pattern       *prev; //!< for list
        private: void          *self; //!< pointer to base class
        public:  const uint32_t uuid; //!< identifier

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Pattern() noexcept; //!< cleanup
        protected:
            explicit Pattern(const uint32_t ) noexcept; //!< setup
            explicit Pattern(const Pattern &) noexcept; //!< copy

        private:
            Y_DISABLE_ASSIGN(Pattern);
        };

    }

}

#endif
