
//! \file

#ifndef Y_Jive_Parser_Included
#define Y_Jive_Parser_Included 1

#include "y/jive/grammar.hpp"
#include "y/jive/module.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! a Parser is a Grammar with a synchronized Lexer
        //
        //
        //______________________________________________________________________
        class Parser : public Grammar
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Parser() noexcept;

            template <typename ID> inline
            explicit Parser(const ID &id) :
            Grammar(id), lexer(id)
            {
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! create a terminal
            template <typename ID, typename RX>
            const Rule & term(const ID &id, const RX &rx)
            {
                const Lexical::Action &action = lexer.emit(id,rx);
                return add( new Term(action.name,Term::IsRegular,action.motif->isUnivocal()) );
            }

            //! parse the module
            XNode * operator()(Jive::Module *);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        protected:
            Lexer lexer;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
        };

    }

}

#endif
