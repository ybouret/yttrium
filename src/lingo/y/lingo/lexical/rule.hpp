


//! \file

#ifndef Y_Lingo_Lexical_Rule_Included
#define Y_Lingo_Lexical_Rule_Included 1

#include "y/lingo/pattern.hpp"
#include "y/lingo/lexical/outcome.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //! Generic Outcome returning callback to execute for best pattern
            //
            //__________________________________________________________________
            typedef Functor<Outcome,TL1(const Token&)>   Callback;
            
            //__________________________________________________________________
            //
            //
            //
            //! Rule to produce Unit/Action based on a STRONG pattern
            //
            //
            //__________________________________________________________________
            class Rule : public Quantized, public Entity
            {
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
            protected:
                //! setup rule, need to protect motif
                explicit Rule(const Caption & _rname,
                              Pattern * const _motif,
                              const Callback &_xcode) noexcept;

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
                template <typename NAME,
                typename HOST,
                typename METHOD>
                static Rule * Create(const NAME     & _rname,
                                     Pattern * const  _motif,
                                     HOST           &host,
                                     METHOD          meth)
                {
                    AutoPtr<Pattern> motif(_motif); assert(motif.isValid());
                    const Caption    rname(_rname);
                    const Callback   xcode(&host,meth);
                    if(motif->feeble()) ErrorFeeblePattern(*rname);
                    Rule * const     rule = new Rule(rname,& *motif, xcode);
                    motif.relax();
                    return rule;
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Pattern  * const motif;  //!< triggering motif
                Callback               xcode;  //!< to execute
                Rule *                 next;   //!< for list
                Rule *                 prev;   //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rule);
                static void ErrorFeeblePattern(const String &rid);
            };

            typedef CxxListOf<Rule> Rules; //!< alias
        }

    }

}

#endif

