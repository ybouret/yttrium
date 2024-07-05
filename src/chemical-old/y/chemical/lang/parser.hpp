//! \file

#ifndef Y_Chemical_Lang_Parser_Included
#define Y_Chemical_Lang_Parser_Included 1


#include "y/jive/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! parser of species/equlibria
        //
        //
        //______________________________________________________________________
        class Parser : public Jive::Parser
        {
        public:
            explicit Parser(const char *); //!< setup with identifier
            virtual ~Parser() noexcept;    //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
    }

}

#endif

