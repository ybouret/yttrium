//! \file

#ifndef Y_CSV_Parser_Included
#define Y_CSV_Parser_Included 1

#include "y/jive/parser.hpp"

namespace Yttrium
{

    namespace CSV
    {

        //______________________________________________________________________
        //
        //
        //
        //! a Parser for CSV
        //
        //
        //______________________________________________________________________
        class Parser : public Jive::Parser
        {
        public:

            explicit Parser();          //!< setup
            virtual ~Parser() noexcept; //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };
    }

}

#endif

