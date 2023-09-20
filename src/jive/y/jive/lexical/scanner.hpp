//! \file

#ifndef Y_Lexical_Scanner_Included
#define Y_Lexical_Scanner_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/pattern/dictionary.hpp"
#include "y/jive/lexical/unit.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/functor.hpp"

namespace Yttrium
{
    namespace Jive
    {

        namespace Lexical
        {


            

            class Scanner : public Tag
            {
            public:
                template <typename TAG>
                explicit Scanner(TAG &usr) :
                Tag(usr),
                dict( new Dictionary() )
                {

                }

                virtual ~Scanner() noexcept;


                ArcPtr<Dictionary> dict;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
            };
        }
        
    }
}

#endif
