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
            template <typename NAME> inline
            explicit Parser(const NAME & _name) :
            Syntax::Grammar(_name),
            lexer(name)
            {
            }

            virtual ~Parser() noexcept;

            template <typename ID,typename RX> inline
            const Rule & term(const ID & id, const RX &rx)
            {
                Syntax::Terminal::Kind kind = Syntax::Terminal::Standard;
                lexer.emit(id,rx).motif;
            }


            XNode * operator()(Source &source);


            Lexer lexer;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif

