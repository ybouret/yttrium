
//! \file

#ifndef Y_Chemical_Weasel_Parser_Included
#define Y_Chemical_Weasel_Parser_Included 1

#include "y/chemical/weasel.hpp"
#include "y/lingo/parser.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        //______________________________________________________________________
        //
        //
        //
        //! Parser of Weasel
        //
        //
        //______________________________________________________________________
        class Weasel:: Parser : public Lingo::Parser
        {
        public:
            explicit Parser(const Lingo::Caption &); //!< setup
            virtual ~Parser() noexcept;              //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif


