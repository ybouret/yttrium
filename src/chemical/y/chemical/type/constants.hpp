
//! \file

#ifndef Y_Chemical_Types_Constants_Included
#define Y_Chemical_Types_Constants_Included 1

#include "y/chemical/type/defs.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/counted.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Chemical
    {
     
        typedef Vector<XReal,Memory::Dyadic> XVectorType;

        class XVector : public Object, public Counted, public XVectorType
        {
        public:
            explicit XVector() noexcept {}
            virtual ~XVector() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(XVector);
        };


        class Constants : public ArcPtr<XVector>
        {
        public:
            explicit Constants() : ArcPtr<XVector>( new XVector() ) {}
            Constants(const Constants &other) noexcept : ArcPtr<XVector>(other) {}
            virtual ~Constants() noexcept {}

        private:
            Y_DISABLE_ASSIGN(Constants);
        };


    }
}

#endif
