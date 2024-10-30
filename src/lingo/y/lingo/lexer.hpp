
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
            template <typename SCANNER>
            SCANNER & decl(SCANNER * const scan) {
                assert(0!=scan);
                const ScanPtr ps( scan );
                mustInsert(ps);
                return *scan;
            }


            void restart() noexcept;
            void call(const Caption &);
            void back();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            Scanner *scanner;  //!< active scanner
            Lexemes  lexemes;  //!< cache of scanned lexemes
            History  history;  //!< for scanner call/back
            ScanSet  scanners; //!< existing scanners

            void     initialize();                //!< record this into scanner
            void     mustInsert(const ScanPtr &); //!< must insert new scanner
            
        };

    }

}

#endif

