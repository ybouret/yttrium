
//! \file


#ifndef Y_Chemical_Parser_Included
#define Y_Chemical_Parser_Included 1

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
        //! Parser
        //
        //
        //______________________________________________________________________
        class Weasel:: Parser : public Lingo::Parser
        {
        public:

            explicit Parser();
            virtual ~Parser() noexcept;

            //! parse and cleanup module
            XNode * preprocess(Lingo::Module * const);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            void processRegExp(XList &target, const String &regexp);
        };


    }
}

#endif

