
//! \file

#ifndef Y_Lingo_Lexer_Included
#define Y_Lingo_Lexer_Included 1

#include "y/lingo/lexical/scanner.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Lexer :
        public Dictionary,
        public Lexical::Scanner
        {
        public:
            typedef Lexical::Scanner          Scanner;
            typedef Scanner::Pointer          ScanPtr;
            typedef SuffixSet<String,ScanPtr> ScanSet;

            template <typename CAPTION> inline
            Lexer(const CAPTION &lxid) :
            Dictionary(),
            Scanner(lxid,*this),
            scanners()
            {
            }

            virtual ~Lexer() noexcept;

            Scanner & operator[](const int);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            ScanSet scanners;

            void initialize();
            
        };

    }

}

#endif

