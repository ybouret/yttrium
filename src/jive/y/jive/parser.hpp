
//! \file

#ifndef Y_Jive_Parser_Included
#define Y_Jive_Parser_Included 1

#include "y/jive/grammar.hpp"
#include "y/jive/module.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Parser : public Grammar
        {
        public:
            virtual ~Parser() noexcept;

            template <typename ID> inline
            explicit Parser(const ID &id) :
            Grammar(id),
            lexer(id)
            {
            }

            XNode * operator()(Jive::Module *);

            template <typename ID, typename RX>
            const Rule & term(const ID &id, const RX &rx)
            {
                
            }



            Lexer lexer;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif
