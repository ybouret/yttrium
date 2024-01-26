
//! \file

#ifndef Y_Fit_Variable_Interface_Included
#define Y_Fit_Variable_Interface_Included 1

#include "y/string.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {

            class Variable : public String
            {
            public:
                typedef ArkPtr<String,Variable> Handle;
                
                explicit Variable(const String &, const size_t);
                explicit Variable(const char   *, const size_t);
                explicit Variable(const char    , const size_t);
                virtual ~Variable() noexcept;

                const String & key() const noexcept;

                const size_t indx;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Variable);
            };

        }

    }

}


#endif

