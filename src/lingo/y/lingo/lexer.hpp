
//! \file

#ifndef Y_Lingo_Lexer_Included
#define Y_Lingo_Lexer_Included 1

#include "y/lingo/lexical/analyzer.hpp"
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
        class Lexer : public Dictionary, public Lexical::Analyzer
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Lexical::Analyzer                   Analyzer; //!< alias
            typedef SuffixSet<String,Analyzer::Pointer> Database; //!< alias
            typedef Small::SoloLightList<Analyzer>      History;  //!< alias

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
            Analyzer(lxid,*this,*this),
            analyzer(this),
            lexemes(),
            history(),
            analyzers()
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


            //! declare a newly created analyzer
            template <typename ANALYZER>
            ANALYZER & decl(ANALYZER * const newAnalyzer) {
                assert(0!=newAnalyzer);
                const Analyzer::Pointer _( newAnalyzer );
                mustInsert(_);
                return *newAnalyzer;
            }


            //! release lexemes, free history, scanner to this
            void restart() noexcept;

            //! change scanner by its name
            void call(const Caption &);

            //! back from current scanner
            void back();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Lexer);
            Analyzer *analyzer;  //!< active analyzer
            Lexemes   lexemes;   //!< cache of scanned lexemes
            History   history;   //!< for scanner call/back
            Database  analyzers; //!< existing analyzers

            void     initialize();                          //!< record this into scanner
            void     mustInsert(const Analyzer::Pointer &); //!< must insert new analyzer

        };

    }

}

#endif

