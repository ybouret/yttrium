
//! \file


#ifndef Y_Chemical_FormulaCompiler_Included
#define Y_Chemical_FormulaCompiler_Included 1

#include "y/lingo/syntax/translator.hpp"
#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Base class for Formula Compilers
        //
        //
        //______________________________________________________________________
        class FormulaCompiler :  public Lingo::Syntax::Translator
        {
        public:
            typedef Vector<String,Memory::Pooled> Strings; //!< alias

        protected:
            explicit FormulaCompiler(const char * const);  //!< setup

        public:
            virtual ~FormulaCompiler() noexcept;           //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormulaCompiler);
        };
    }

}

#endif
