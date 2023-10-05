
//! \file

#ifndef Y_Jive_Syntax_Grammar_Included
#define Y_Jive_Syntax_Grammar_Included 1

#include "y/jive/syntax/terminal.hpp"
#include "y/jive/syntax/compound/aggregate.hpp"
#include "y/jive/syntax/compound/alternate.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //__________________________________________________________________
        //
        //
        //
        //! Grammar is a set of rules
        //
        //
        //__________________________________________________________________
        class Grammar : public Entity
        {
        public:
            //______________________________________________________________
            //
            //
            // Definitions
            //
            //______________________________________________________________
            typedef Syntax::Rule         Rule;
            typedef Syntax::Aggregate    Agg;
            typedef Syntax::Alternate    Alt;
            

            //______________________________________________________________
            //
            //
            // C++
            //
            //______________________________________________________________
            virtual ~Grammar() noexcept; //!< cleanup

            //! setup
            template <typename ID> inline
            explicit Grammar(const ID &id) :
            Entity(id), code( Initialize(name) )
            {}

            //______________________________________________________________
            //
            //
            // Methods
            //
            //______________________________________________________________

            //! generic add a newly created rule
            template <typename RULE> inline
            RULE & add(RULE *rule)
            {
                assert(0!=rule);
                add( static_cast<Rule *>(rule) );
                return *rule;
            }

            void         topLevel(const Rule &);            //!< set top-level rule
            const Rule & topLevel()                  const; //!< get top-level rule
            const Rule & operator[](const String & ) const; //!< get rule by name


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Grammar);
            class Code;
            Code *code;

            static Code *Initialize(const Tag &);
            void         add(Rule *);
        };


    }
}

#endif
