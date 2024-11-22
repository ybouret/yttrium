//! \file

#ifndef Y_Lingo_Syntax_Internal_Included
#define Y_Lingo_Syntax_Internal_Included 1

#include "y/lingo/syntax/rule.hpp"

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
            //! Internal rule to build more complex motifs
            //
            //
            //__________________________________________________________________
            class Internal : public Rule
            {

            protected:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with name and uuid
                template <typename NAME>
                explicit Internal(const NAME &    _name,
                                  const uint32_t  _uuid,
                                  Grammar * const _host) :
                Rule(_name,_uuid,_host)
                {
                }

            public:
                //! cleanup
                virtual ~Internal() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Internal);
            };

        }

    }

}


#endif

