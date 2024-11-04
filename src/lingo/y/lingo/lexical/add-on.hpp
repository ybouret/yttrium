
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

            class AddOn : public Analyzer
            {
            public:
                typedef ArkPtr<String,const AddOn> Handle;
                static const char * const BackLabel;

                enum EndOfSourcePolicy
                {
                    AcceptEndOfSource,
                    RejectEndOfSource
                };


                virtual ~AddOn() noexcept;


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
                    back(BackLabel,leaveExpr, *this, & AddOn::onLeave, leaveSpot);


                    //----------------------------------------------------------
                    //
                    // prepare call rule for lexer
                    //
                    //----------------------------------------------------------
                    const Hook       hook(this, & AddOn::onEnter);
                    const Callback   xcode = makeCall(name,hook,Unit::Bulk);
                    AutoPtr<Pattern> motif = compile(enterExpr);
                    trigger(rootLexer,Rule::Create(name,motif,xcode));
                }

                const EndOfSourcePolicy policy;

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
