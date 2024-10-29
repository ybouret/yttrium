//! \file

#ifndef Y_Lingo_Lexical_Unit_Included
#define Y_Lingo_Lexical_Unit_Included 1

#include "y/lingo/token.hpp"

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
            //! Unit produced from a rule = named token
            //
            //
            //__________________________________________________________________
            class Unit : public Object, public Entity, public Token
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________

                //! possible types for rule/outcome
                enum Type
                {
                    Regular, //!< regular lexeme
                    Control  //!< control lexeme
                };

                //! possible spots for rule/outcome
                enum Spot
                {
                    Bulk, //!< default spot
                    Endl  //!< must propagate newLine to source
                };

                //! possible feats for regular lexemes
                enum Feat
                {
                    Emit, //!< is emitted
                    Drop  //!< is silently discardes
                };

                //! info for regular lexeme
                struct RegularInfo {
                    Feat feat; //!< emit/drop
                };

                //! info for control lexeme
                struct ControlInfo {
                    
                };


                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup from rule and token's creation context
                explicit Unit(const Entity  &,
                              const Context &) noexcept;


                virtual ~Unit() noexcept; //!< cleanup
                Y_OSTREAM_PROTO(Unit);    //!< display

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Unit *        next; //!< for list
                Unit *        prev; //!< for list
                const Context info; //!< productiom info

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Unit);
            };
        }

        typedef Lexical::Unit     Lexeme;     //!< alias
        typedef CxxListOf<Lexeme> Lexemes;    //!< alias
    }

}

#endif

