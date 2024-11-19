//! \file

#ifndef Y_JSON_Compiler_Included
#define Y_JSON_Compiler_Included 1

#include "y/json/value.hpp"
#include "y/lingo/module.hpp"

namespace Yttrium
{
    namespace JSON
    {
        //______________________________________________________________________
        //
        //
        //
        //! Compile module into Value
        //
        //
        //______________________________________________________________________
        class Compiler
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Compiler();          //!< setup parser+linker
            virtual ~Compiler() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void load(Value &jv, Lingo::Module * const); //!< load into jv

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            class Code;
            Code *code;
        };
    }
}

#endif
