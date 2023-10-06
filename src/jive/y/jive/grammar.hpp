
//! \file

#ifndef Y_Jive_Syntax_Grammar_Included
#define Y_Jive_Syntax_Grammar_Included 1

#include "y/jive/syntax/terminal.hpp"
#include "y/jive/syntax/compound/aggregate.hpp"
#include "y/jive/syntax/compound/alternate.hpp"
#include "y/jive/syntax/wildcard/option.hpp"
#include "y/jive/syntax/wildcard/repeat.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! Grammar is a set of rules
        //
        //
        //______________________________________________________________________
        class Grammar : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Syntax::Rule         Rule;  //!< alias
            typedef Syntax::Aggregate    Agg;   //!< alias
            typedef Syntax::Alternate    Alt;   //!< alias
            typedef Syntax::Option       Opt;   //!< alias
            typedef Syntax::Repeat       Rep;   //!< alias
            typedef Syntax::XNode        XNode; //!< alias
            typedef Syntax::XTree        XTree; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Grammar() noexcept; //!< cleanup

            //! setup with name
            template <typename ID> inline
            explicit Grammar(const ID &id) :
            Entity(id), code( Initialize(name) )
            {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! parse UNTOUCHED lexer using source
            XNode *parse(Lexer &lexer, Source &source);


            //! generic add a newly created rule
            template <typename RULE> inline
            RULE & add(RULE *rule)
            {
                assert(0!=rule);
                ins( static_cast<Rule *>(rule) );
                return *rule;
            }

            void         validate();                        //!< validate and lock grammar
            void         topLevel(const Rule &);            //!< set top-level rule
            const Rule & topLevel()                  const; //!< get top-level rule
            const Rule & operator[](const String & ) const; //!< get rule by name

            //__________________________________________________________________
            //
            //
            // Design helpers
            //
            //__________________________________________________________________
            const Rule & zom(const Rule &);                                 //!< Zero Or More rule
            const Rule & oom(const Rule &);                                 //!< One  Or More rule
            const Rule & cat(const Rule  &a, const Rule &b);                //!< catenate a and b
            const Rule & cat(const Rule  &a, const Rule &b, const Rule &c); //!< catenate a and b and c
            const Rule & pick(const Rule &a, const Rule &b);                //!< pick a or b
            const Rule & pick(const Rule &a, const Rule &b, const Rule &c); //!< pick a or b of c
            const Rule & opt(const Rule &);                                 //!< make optional rule


            void    graphViz(OutputStream &fp) const; //!< output GraphViz
            XMLog & getXMLog()        const noexcept; //!< get code xml
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            class Code;
            Code *code;

            static Code *  Initialize(const Tag &);
            void           ins(Rule *);
            static String *UID(const Syntax::Manifest &, const char sep);
            const Rule    &agg(Syntax::Manifest &);
            const Rule    &alt(Syntax::Manifest &);

        };


    }
}

#endif
