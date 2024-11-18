//! \file

#ifndef Y_Lingo_Parser_Included
#define Y_Lingo_Parser_Included 1

#include "y/lingo/lexer.hpp"
#include "y/lingo/syntax/grammar.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Parser is a Grammar with a Lexer
        //
        //
        //______________________________________________________________________
        class Parser : public Syntax::Grammar
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Syntax::Terminal Terminal; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with name
            template <typename NAME> inline
            explicit Parser(const NAME & _name) :
            Syntax::Grammar(_name),
            lexer(name)
            {
            }

            virtual ~Parser() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! create a terminal with a given role
            template <typename UUID,typename EXPR,Terminal::Role ROLE> inline
            const Rule & term_(const UUID & uuid,
                               const EXPR & expr)
            {
                const Lexical::Rule &rule = lexer.emit(uuid,expr);
                const Terminal::Kind kind = rule.motif->univocal() ? Terminal::Univocal : Terminal::Standard;
                const Caption       &r_id = rule.name;
                try { return term__(r_id,kind,ROLE); }
                catch(...) { lexer.cut(rule); throw; }
            }

            //! create a semantic terminal
            template <typename UUID,typename EXPR> inline
            const Rule & term(const UUID & uuid,
                              const EXPR & expr)
            {
                return term_<UUID,EXPR,Terminal::Semantic>(uuid,expr);
            }

            //! create a semantic terminal
            template <typename EXPR> inline
            const Rule & term(const EXPR &expr) {
                term(expr,expr);
            }

            //! create a dividing terminal
            template <typename UUID,typename EXPR> inline
            const Rule & mark(const UUID & uuid,
                              const EXPR & expr)
            {
                return term_<UUID,EXPR,Terminal::Dividing>(uuid,expr);
            }

            //! create a dividing terminal
            template <typename EXPR> inline
            const Rule & mark(const EXPR &expr) {
                mark(expr,expr);
            }

            

            //! restart lexer and process source
            XNode * operator()(Source &source);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Lexer lexer; //!< internal lexer, may be used to debug

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif

