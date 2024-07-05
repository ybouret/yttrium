

//! \file

#ifndef Y_Chemical_Weasel_Parser_Included
#define Y_Chemical_Weasel_Parser_Included 1

#include "y/chemical/library.hpp"
#include "y/jive/parser.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {

            typedef Jive::Syntax::XNode XNode;
            
            //! WEASEL is Equilibria And Species Elementary Language
            class Parser : public Jive::Parser
            {
            public:
                
                explicit Parser(const char * const);
                virtual ~Parser() noexcept;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }

    }

}

#endif
