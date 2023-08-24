//! \file

#ifndef Y_Jive_Position_Included
#define Y_Jive_Position_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Position
        {
        public:
            explicit Position() noexcept;
            virtual ~Position() noexcept;

            void newChar() noexcept; //!< increase ncol
            void newLine() noexcept; //!< incrase  line, ncol=1

            const unsigned line; //!< current line
            const unsigned ncol; //!< current column

        private:
            Y_DISABLE_ASSIGN(Position);
        };

    }

}

#endif
