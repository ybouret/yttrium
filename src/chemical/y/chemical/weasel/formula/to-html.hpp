
//! \file


#ifndef Y_Chemical_FormulaToHTMl_Included
#define Y_Chemical_FormulaToHTMl_Included 1

#include "y/chemical/weasel.hpp"
#include "y/chemical/weasel/formula/compiler.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Transform parsed formula into univocal test
        //
        //
        //______________________________________________________________________
        class Weasel:: FormulaToHTML : public FormulaCompiler
        {
        public:
            static const char * const CallSign; //!< "FormulaToHTML"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FormulaToHTML();            //!< setup
            virtual ~FormulaToHTML() noexcept;   //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! build name and fetch
            const String *get(const XNode &node);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(FormulaToHTML);

        };
    }

}

#endif
