//! \file

#ifndef Y_Lingo_Lexical_Analyzer_Included
#define Y_Lingo_Lexical_Analyzer_Included 1

#include "y/lingo/lexical/scanner.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        class Lexer;

        namespace Lexical
        {
            typedef Functor<void,TL1(const Token&)>   Hook; //!< function called during Control unit

            //__________________________________________________________________
            //
            //
            //
            //! an Analyzer is a Scanner attached to a Lexer
            //
            //
            //__________________________________________________________________
            class Analyzer : public Scanner
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef ArkPtr<String,Analyzer> Pointer;     //!< alias
                static const char * const       Back;        //!< "back"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                //! setup from name and shared dictionary
                template <typename CAPTION>
                explicit Analyzer(const CAPTION          &_name,
                                  Lexer                  &_lexr,
                                  const Dictionary       &_dict) noexcept :
                Scanner(_name,_dict), lexer(_lexr)
                {
                }

                explicit Analyzer(Lexer &, const String     &); //!< setup
                explicit Analyzer(Lexer &, const char * const); //!< setup
                explicit Analyzer(Lexer &, const Caption    &); //!< setup
                virtual ~Analyzer() noexcept;                   //!< cleanup


                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! generic call function to another lexer's analyzer
                /**
                 \param goal  name of the target scanner = name of the rule
                 \param expr  triggering expression
                 \param host  host   for Hook
                 \param meth  method for Hook
                 \param spot  Bulk/Endl to control Source/Module
                 */
                template <
                typename GOAL,
                typename EXPR,
                typename HOST,
                typename METH>
                void  call(const GOAL & goal,
                           const EXPR & expr,
                           HOST       & host,
                           METH         meth,
                           Unit::Spot   spot)
                {
                    const Caption    _goal(goal);
                    const Hook       _hook(&host,meth);
                    const Callback   xcode( makeCall(_goal,_hook,spot) );
                    AutoPtr<Pattern> motif( compile(expr) );
                    add( Rule::Create(_goal, motif, xcode) );
                }

                //! generic "back" function from current analyzer
                /**
                 \param expr  triggering expression
                 \param host  host   for Hook
                 \param meth  method for Hook
                 \param spot  Bulk/Endl to control Source/Module
                 */
                template <
                typename EXPR,
                typename HOST,
                typename METH>
                void back(const EXPR & expr,
                          HOST       & host,
                          METH         meth,
                          Unit::Spot   spot)
                {
                    const Hook       _hook(&host,meth);
                    const Callback   xcode( makeBack(_hook,spot) );
                    AutoPtr<Pattern> motif( compile(expr) );
                    const Caption    rname( Back);
                    add( Rule::Create(rname, motif, xcode) );
                }

            protected:
                //! create a call from lexer's current scanner to goal
                Callback makeCall(const Caption &  goal,
                                  const Hook    &  hook,
                                  const Unit::Spot spot);



            public:
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Lexer &lexer; //!< parent lexer where AddOn is recorded
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Analyzer);

                //! coming back from lexer's current scanner to caller
                Callback makeBack(const Hook    &  hook,
                                  const Unit::Spot spot);


            };
        }

    }

}

#endif

