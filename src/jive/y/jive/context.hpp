//! \file

#ifndef Y_Jive_Context_Included
#define Y_Jive_Context_Included 1

#include "y/jive/tag.hpp"
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


            //! initialize a context from a tag
            template <typename TAG> inline
            Context(TAG &usr, const AsTag_ &) :
            tag(usr), line(1), column(1)
            {
            }

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
            const Tag      tag;    //!< identifier
            const unsigned line;   //!< current line
            const unsigned column; //!< current column
            
        private:
            Y_DISABLE_ASSIGN(Context);
        };
    }

}

#endif

