
//! \file

#ifndef Y_Jive_Pattern_RegExp_Compiler_Included
#define Y_Jive_Pattern_RegExp_Compiler_Included 1

#include "y/jive/pattern/dictionary.hpp"
#include "y/singleton.hpp"

namespace  Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! Regular Expression Compiler
        //
        //______________________________________________________________________
        class RegExpCompiler : public Singleton<RegExpCompiler>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                  //!< "RegExpCompiler"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 10;  //!< life time


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! compile the given regular expression with optional dictionary
            Pattern *            operator()(const String &rx, const Dictionary *dict) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Dictionary posixDict; //!< registered posix for '[:expr:]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RegExpCompiler);
            friend class Singleton<RegExpCompiler>;
            explicit RegExpCompiler();
            virtual ~RegExpCompiler() noexcept;

        };
    }
}

#endif

