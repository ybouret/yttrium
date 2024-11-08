//! \file

#ifndef Y_Lingo_Syntax_Grammar_Included
#define Y_Lingo_Syntax_Grammar_Included 1

#include "y/lingo/syntax/rules.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //
            //! Grammar is a list of rule
            //
            //
            //__________________________________________________________________
            class Grammar : public Entity, public Proxy<const Rules>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup empty with given name
                template <typename NAME> inline
                explicit Grammar(const NAME & _name) :
                Entity(_name,AsCaption),
                rules(),
                locked(false)
                {
                }

                //! cleanup
                virtual ~Grammar() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void         add(Rule * const  rule); //!< add a new rule
                const Rule & top() const;             //!< query top rule
                void         top(const Rule &);       //!< set top rule

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Rules rules;
                virtual ConstInterface & surrogate() const noexcept;
                
            public:
                const bool  locked; //!< status
            };

        }

    }

}

#endif
