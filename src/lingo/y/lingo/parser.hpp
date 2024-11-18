//! \file

#ifndef Y_Lingo_Parser_Included
#define Y_Lingo_Parser_Included 1

#include "y/lingo/lexer.hpp"
#include "y/lingo/syntax/grammar.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Parser : public Syntax::Grammar
        {
        public:
            typedef Syntax::Terminal Term;

            template <typename NAME> inline
            explicit Parser(const NAME & _name) :
            Syntax::Grammar(_name),
            lexer(name)
            {
            }

            virtual ~Parser() noexcept;

            template <typename UUID,typename EXPR,Term::Role ROLE> inline
            const Rule & term_(const UUID & uuid,
                               const EXPR & expr)
            {
                const Lexical::Rule &rule = lexer.emit(uuid,expr);
                const Term::Kind     kind = rule.motif->univocal() ? Term::Univocal : Term::Standard;
                const Caption       &r_id = rule.name;
                try { return term__(r_id,kind,ROLE); }
                catch(...)
                {
                    lexer.cut(rule);
                    throw;
                }
            }

            


            XNode * operator()(Source &source);


            Lexer lexer;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif

