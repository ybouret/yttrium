
//! \file


#ifndef Y_Chemical_Initial_Axioms_Included
#define Y_Chemical_Initial_Axioms_Included 1

#include "y/chemical/type/xcode.hpp"
#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {
            class Axioms : public Quantized, public Counted, public Entity
            {
            public:
                typedef ArkPtr<String,Axioms> Pointer;
                explicit Axioms(const XCode &xcode);
                virtual ~Axioms() noexcept;
                
            private:
                XCode xcode;
            };
        }
    }
}

#endif

