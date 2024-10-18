//! \file

#ifndef Y_Lingo_Context_Included
#define Y_Lingo_Context_Included 1

#include "y/lingo/caption.hpp"

namespace Yttrium
{
    class Exception;

    namespace Lingo
    {

        class Context
        {
        public:

            // C++

            //! generic constructor
            template <typename CAPTION>
            inline Context(const CAPTION &_, const AsCaption_ &) :
            caption(_), line(1), column(1)
            {
            }

            explicit Context(const Context &) noexcept; //!< copy
            virtual ~Context()                noexcept; //!< cleanup

            // Methods

            //! stamp exception with 'caption:line:colum: '
            Exception & stamp(Exception &) const noexcept;

            void newChar() noexcept; //!< increase column
            void newLine() noexcept; //!< incrase  line, column=1

            // Members
            const Caption       caption;
            const unsigned long line;
            const unsigned long column;

        private:
            Y_DISABLE_ASSIGN(Context);
        };

    }

}

#endif

