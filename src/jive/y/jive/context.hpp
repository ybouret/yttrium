//! \file

#ifndef Y_Jive_Context_Included
#define Y_Jive_Context_Included 1

#include "y/jive/mark.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Input Context
        //
        //
        //______________________________________________________________________
        class Context
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            Context(String        *) noexcept; //!< setup
            Context(const Mark    &) noexcept; //!< setup
            Context(const Context &) noexcept; //!< copy
            virtual ~Context()       noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! stamp exception with 'mark:line:ncol: '
            Exception & stamp(Exception &) const noexcept;

            void newChar() noexcept; //!< increase ncol
            void newLine() noexcept; //!< incrase  line, ncol=1

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Mark     mark; //!< identifier
            const unsigned line; //!< current line
            const unsigned ncol; //!< current column
            
        private:
            Y_DISABLE_ASSIGN(Context);
        };
    }

}

#endif

