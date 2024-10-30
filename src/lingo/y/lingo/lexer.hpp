
//! \file

#ifndef Y_Lingo_Lexer_Included
#define Y_Lingo_Lexer_Included 1

#include "y/lingo/lexical/scanner.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        //______________________________________________________________________
        //
        //
        //
        //! main scanner with sub-scanners to produce lexemes
        //
        //
        //______________________________________________________________________
        class Lexer : public Dictionary, public Lexical::Scanner
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Lexical::Scanner          Scanner;
            typedef Scanner::Pointer          ScanPtr;
            typedef SuffixSet<String,ScanPtr> ScanSet;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with name
            template <typename CAPTION> inline
            Lexer(const CAPTION &lxid) :
            Dictionary(),
            Scanner(lxid,*this),
            scanners()
            {
                initialize();
            }

            //! cleanup
            virtual ~Lexer() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! query/create named scanner
            template <typename UUID> inline
            Scanner & operator[](const UUID &uuid)
            {
                const Caption label(uuid);
                return query(label);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            ScanSet scanners;

            void     initialize();
            void     mustInsert(const ScanPtr &);
            Scanner &query(const Caption &);

        };

    }

}

#endif

