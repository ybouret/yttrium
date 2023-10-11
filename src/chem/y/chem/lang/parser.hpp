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
        //! Parse species/equilibria
        //
        //
        //______________________________________________________________________
        class Parser : public Jive::Parser
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Jive::Syntax::XNode XNode; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Parser();           //!< create parse/lexer
            virtual ~Parser() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Jive::Tag reac; //!< "REAC"
            const Jive::Tag prod; //!< "PROD"
            const Jive::Tag plus; //!< "+"
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            XNode *prune(XNode *) noexcept;
            void   scrub(XNode *) noexcept;
        };

    }
}

#endif

