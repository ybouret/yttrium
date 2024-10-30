
//! \file

#ifndef Y_Lingo_Lexer_Included
#define Y_Lingo_Lexer_Included 1

#include "y/lingo/lexical/scanner.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/data/small/light/list/solo.hpp"

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
            typedef Lexical::Scanner              Scanner; //!< alias
            typedef Scanner::Pointer              ScanPtr; //!< alias
            typedef SuffixSet<String,ScanPtr>     ScanSet; //!< alias
            typedef Small::SoloLightList<Scanner> History; //!< alias

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
            scanner(this),
            lexemes(),
            history(),
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

            void restart() noexcept;
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            Scanner *scanner;  //!< active scanner
            Lexemes  lexemes;  //!< cache of scanned lexemes
            History  history;  //!< for scanner call/back
            ScanSet  scanners; //!< existing scanners

            void     initialize();                //!< record this into scanner
            void     mustInsert(const ScanPtr &); //!< must insert new scanner
            Scanner &query(const Caption &);      //!< query/create named scanner

        };

    }

}

#endif

