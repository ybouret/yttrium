//! \file

#ifndef Y_Container_Lexicon_Included
#define Y_Container_Lexicon_Included 1

#include "y/type/args.hpp"

namespace Yttrium
{

    template <typename T>
    class Lexicon
    {
    public:
        inline virtual ~Lexicon() noexcept {}

        

    protected:
        inline explicit Lexicon() noexcept {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Lexicon);
    };

}

#endif

