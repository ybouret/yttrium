
//! \file

#ifndef Y_Lexical_Unit_Included
#define Y_Lexical_Unit_Included 1

#include "y/jive/token.hpp"


namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {
            //__________________________________________________________________
            //
            //
            //
            //! a Unit is a Token from a Context
            //
            //
            //__________________________________________________________________
            class Unit : public Object, public Context, public Token
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Unit(const Context &) noexcept; //!< empty unit from context
                Unit(const Unit &);                      //!< copy context/content
                virtual ~Unit() noexcept;                //!< cleanup

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Unit *next; //!< for list
                Unit *prev; //!< for list


            private:
                Y_DISABLE_ASSIGN(Unit);
            };
            

        }

    }
}

#endif
