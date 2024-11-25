
//! \file

#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1

#include "y/chemical/type/lang.hpp"
#include "y/chemical/type/memory.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        class Formula
        {
        public:
            class Linker;
            
            explicit Formula(XNode * const _xnode);
            explicit Formula(const String &expr);
            explicit Formula(const char * const expr);
            
            virtual ~Formula() noexcept;

            const Title          title;
            const AutoPtr<XNode> xnode;
            const int            z;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Formula);
            
        };

    }

}

#endif

