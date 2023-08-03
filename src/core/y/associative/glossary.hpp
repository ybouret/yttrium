
//! \file

#ifndef Y_Associative_Glossary_Included
#define Y_Associative_Glossary_Included 1

#include "y/associative/interface.hpp"

namespace Yttrium
{

    template <typename KEY, typename T>
    class Glossary : public Associative<KEY,T>
    {
    protected:
        inline explicit Glossary() noexcept {}

    public:
        inline virtual ~Glossary() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Glossary);
    };

}

#endif
