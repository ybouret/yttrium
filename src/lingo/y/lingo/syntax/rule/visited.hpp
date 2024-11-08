
//! \file

#ifndef Y_Lingo_Syntax_Rule_Visited_Included
#define Y_Lingo_Syntax_Rule_Visited_Included 1

#include "y/lingo/syntax/rule.hpp"
#include "y/associative/address-book.hpp"

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
            //! Probing visited rules from one rule
            //
            //
            //__________________________________________________________________
            class Visit  : public AddressBook
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Visit();           //!< setup empty
                virtual ~Visit() noexcept;  //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! recursive visti
                void operator()(const Rule &);


            private:
                Y_DISABLE_ASSIGN(Visit);
            };
        }

    }

}

#endif

