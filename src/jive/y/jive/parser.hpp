
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

            //! setup with id
            template <typename ID> inline
            explicit Parser(const ID &id) :
            Grammar(id), lexer(name)
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
                return term_(lexer.emit(id,rx),Term::IsRegular);
            }

            //! create a terminal name=expression
            template < typename RX>
            const Rule & term(const RX &rx)
            {
                return term_(lexer.emit(rx,rx),Term::IsRegular);
            }


            //! create a divider
            template <typename ID, typename RX>
            const Rule & mark(const ID &id, const RX &rx)
            {
                return term_(lexer.emit(id,rx),Term::IsDivider);
            }

            //! create a divider, name=expression
            template <typename RX>
            const Rule & mark(const RX &rx)
            {
                return term_(lexer.emit(rx,rx),Term::IsDivider);
            }


            //! make a terminal from a plugin
            template <typename PLUGIN, typename ID>
            const Rule & plug(const ID &id)
            {
                const Lexical::Scanner &scanner = lexer.plug<PLUGIN,ID>(id);
                return add( new Term(scanner.name,Term::IsRegular,false) );
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
            Lexer lexer; //!< internal lexer


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            const Rule & term_(const Lexical::Action &action, const Term::Property &ppty);

        };

    }

}

#endif
