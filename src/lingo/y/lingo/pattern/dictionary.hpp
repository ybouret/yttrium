//! \file

#ifndef Y_Lingo_Pattern_Dictionary_Included
#define Y_Lingo_Pattern_Dictionary_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        

        //______________________________________________________________________
        //
        //
        //
        //! Dictionary of shared precompiled patterns
        //
        //
        //______________________________________________________________________
        class Dictionary
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Dictionary();
            virtual ~Dictionary() noexcept;
            explicit Dictionary(const Dictionary &) noexcept; //!< shared copy
            Y_OSTREAM_PROTO(Dictionary);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void operator()(const String &     name, Pattern * const p); //!< insert new pattern
            void operator()(const char * const name, Pattern * const p); //!< insert new pattern

            Pattern * operator()(const String &)     const; //!< clone named pattern
            Pattern * operator()(const char * const) const; //!< cline named pattern


        private:
            Y_DISABLE_ASSIGN(Dictionary);
            class Code;
            Code *code;
        };
    }

}

#endif

