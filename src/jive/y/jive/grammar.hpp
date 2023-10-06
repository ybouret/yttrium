
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
            typedef Syntax::Rule         Rule; //!< alias
            typedef Syntax::Aggregate    Agg;  //!< alias
            typedef Syntax::Alternate    Alt;  //!< alias
            typedef Syntax::Option       Opt;  //!< alias
            typedef Syntax::Repeat       Rep;  //!< alias


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
            
            //! generic add a newly created rule
            template <typename RULE> inline
            RULE & add(RULE *rule)
            {
                assert(0!=rule);
                ins( static_cast<Rule *>(rule) );
                return *rule;
            }

            void         validate();
            void         topLevel(const Rule &);            //!< set top-level rule
            const Rule & topLevel()                  const; //!< get top-level rule
            const Rule & operator[](const String & ) const; //!< get rule by name

            //__________________________________________________________________
            //
            //
            // Design helpers
            //
            //__________________________________________________________________
            const Rule & ZOM(const Rule&); //!< Zero Or More rule
            const Rule & OOM(const Rule&); //!< One  Or More rule


            const Rule &Cat(const Rule &a, const Rule &b);
            const Rule &Cat(const Rule &a, const Rule &b, const Rule &c);
            const Rule &Pick(const Rule &a, const Rule &b);
            const Rule &Pick(const Rule &a, const Rule &b, const Rule &c);

            void graphViz(OutputStream &fp) const;

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
