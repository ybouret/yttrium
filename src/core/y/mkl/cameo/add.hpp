//! \file

#ifndef Y_CamEO_Add_Included
#define Y_CamEO_Add_Included 1

#include "y/mkl/cameo/add/proto.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{

    namespace MKL
    {
        namespace Cameo
        {


            template <typename T, typename ALLOCATOR = Memory::Dyadic>
            class Add :
            public AddProto<T,Proxy<T>,typename Proxy<T>::template FlexibleUnits<ALLOCATOR>::Type>
            {
            public:
                typedef Proxy<T>                                                    ProxyType;
                typedef typename ProxyType::template FlexibleUnits<ALLOCATOR>::Type UnitsType;
                typedef AddProto<T,ProxyType,UnitsType>                             ProtoType;

                explicit Add()      noexcept : ProtoType()  {}
                explicit Add(const size_t n) : ProtoType(n) {}
                virtual ~Add()      noexcept                {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Add);
            };



        }

    }

}
#endif

