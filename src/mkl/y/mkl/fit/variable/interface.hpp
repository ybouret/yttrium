
//! \file

#ifndef Y_Fit_Variable_Interface_Included
#define Y_Fit_Variable_Interface_Included 1

#include "y/string.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/hash/set.hpp"

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
                typedef HashSet<String,Handle>  DB;
                typedef DB::ConstIterator       Iterator;

                explicit Variable(const String &, const size_t);
                explicit Variable(const char   *, const size_t);
                explicit Variable(const char    , const size_t);
                virtual ~Variable() noexcept;
                Y_OSTREAM_PROTO(Variable);
                
                const String & key()       const noexcept;
                //size_t         operator*() const noexcept;



                const size_t indx;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Variable);
            };

        }

    }

}


#endif

