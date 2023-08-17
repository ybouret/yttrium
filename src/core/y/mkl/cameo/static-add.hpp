
//! \file

#ifndef Y_CamEO_Static_Add_Included
#define Y_CamEO_Static_Add_Included 1

#include "y/mkl/cameo/add/proto.hpp"

namespace Yttrium
{

    namespace MKL
    {
        namespace Cameo
        {
            template <typename T, const size_t N>
            class StaticAdd :
            public AddProto<T,Proxy<T>,typename Proxy<T>:: template CompiledUnits<N>::Type>
            {
            public:
                typedef Proxy<T>                                            ProxyType;
                typedef typename Proxy<T>:: template CompiledUnits<N>::Type UnitsType;
                typedef AddProto<T,ProxyType,UnitsType>                     ProtoType;
                explicit StaticAdd() noexcept : ProtoType() {}
                virtual ~StaticAdd() noexcept {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(StaticAdd);
            };
        }

    }

}

#endif
