
//! \file

#ifndef Y_Lingo_Lexical_AddOn_Included
#define Y_Lingo_Lexical_AddOn_Included 1

#include "y/lingo/lexical/analyzer.hpp"

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
            //! an AddOn is a standalone, specialized Analyzer
            //
            //
            //__________________________________________________________________
            class AddOn : public Analyzer
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef ArkPtr<String,const AddOn> Handle; //!< alias

                //! behavior when EOF is reached
                enum EndOfSourcePolicy
                {
                    AcceptEndOfSource, //!< accept early EOF
                    RejectEndOfSource  //!< reject
                };

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~AddOn() noexcept; //!< cleanup


                //! generic setup
                /**
                 \param rootLexer parent lexer
                 \param addOnUUID name of the AddOn to make the rule
                 \param enterExpr triggering expression
                 \param leaveExpr end of AddOn expression
                 \param leaveSpot Bulk/Endl to forward to Source/Module
                 \param eosPolicy behavior if EOF is reached before returning
                 */
                template <
                typename LABEL,
                typename ENTER,
                typename LEAVE> inline
                explicit AddOn(Lexer       &           rootLexer,
                               const LABEL &           addOnUUID,
                               const ENTER &           enterExpr,
                               const LEAVE &           leaveExpr,
                               const Unit::Spot        leaveSpot,
                               const EndOfSourcePolicy eosPolicy) :
                Analyzer(rootLexer,addOnUUID),
                policy(eosPolicy)
                {
                    //----------------------------------------------------------
                    //
                    // and back from this
                    //
                    //----------------------------------------------------------
                    back(leaveExpr, *this, & AddOn::onLeave, leaveSpot);


                    //----------------------------------------------------------
                    //
                    // prepare call rule for lexer
                    //
                    //----------------------------------------------------------
                    const Hook       hook(this, & AddOn::onEnter);
                    const Callback   xcode = makeCall(name,hook,Unit::Bulk);
                    AutoPtr<Pattern> motif = compile(enterExpr);

                    //----------------------------------------------------------
                    //
                    // create call for lexer
                    //
                    //----------------------------------------------------------
                    trigger(rootLexer,Rule::Create(name,motif,xcode));
                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const EndOfSourcePolicy policy; //!< fixed EOF policy

            private:
                Y_DISABLE_COPY_AND_ASSIGN(AddOn);

                void onEnter(const Token &);
                void onLeave(const Token &);
                void trigger(Lexer &lexer, Rule * const rule);



            };

        }

    }

}


#endif
