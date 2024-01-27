
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

            protected:
                explicit Variable(const String &);
                explicit Variable(const char   *);
                explicit Variable(const char    );
            public:
                virtual ~Variable() noexcept;
                Y_OSTREAM_PROTO(Variable);
                
                const String & key() const noexcept;
                virtual size_t idx() const noexcept = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Variable);
                virtual void   dsp(std::ostream &) const = 0;
            };

        }

    }

}


#endif

