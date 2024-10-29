

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
                enum Type
                {
                    Regular,
                    Control
                };

                enum Spot
                {
                    Bulk,
                    Endl
                };

                enum Feat
                {
                    Emit,
                    Drop
                };

                struct RegularInfo {
                    Spot spot;
                    Feat feat;
                };

                struct ControlInfo {
                    Spot spot;
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

                //! cleanup
                virtual ~Unit() noexcept;
                Y_OSTREAM_PROTO(Unit);
                
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

