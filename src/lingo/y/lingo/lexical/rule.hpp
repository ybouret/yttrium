


//! \file

#ifndef Y_Lingo_Lexical_Rule_Included
#define Y_Lingo_Lexical_Rule_Included 1

#include "y/lingo/pattern.hpp"
#include "y/lingo/lexical/unit.hpp"

#include "y/object.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {


            //__________________________________________________________________
            //
            //
            //
            //! Rule to produce Unit/Action based on a STRONG pattern
            //
            //
            //__________________________________________________________________
            class Rule : public Entity
            {
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                //! setup rule, need to protect motif
                explicit Rule(const Caption &,
                              Pattern * const) noexcept;

            public:
                //! cleanup
                virtual ~Rule() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! created checked rule
                template <typename NAME>
                static Rule * Create(const NAME &name, Pattern * const motif)
                {
                    AutoPtr<Pattern> guard( motif ); assert(0!=motif);
                    const Caption    label( name );
                    if( motif->feeble() ) ErrorFeeblePattern(*label);
                    Rule *rule = new Rule(label,motif);
                    guard.relax();
                    return rule;
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Pattern * const motif; //!< triggering patern
                Rule *          next;  //!< for list
                Rule *          prev;  //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
                static void ErrorFeeblePattern(const String &rid);
            };

            typedef CxxListOf<Rule> Rules; //!< alias
        }

    }

}

#endif

