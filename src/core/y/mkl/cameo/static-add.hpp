
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

            //__________________________________________________________________
            //
            //
            //
            //! Add algorithms, compiled memory
            //
            //
            //__________________________________________________________________
            template <typename T, const size_t N>
            class StaticAdd :
            public AddProto<T,Proxy<T>,typename Proxy<T>:: template CompiledUnits<N>::Type>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Proxy<T>                                            ProxyType; //!< alias
                typedef typename Proxy<T>:: template CompiledUnits<N>::Type UnitsType; //!< alias
                typedef AddProto<T,ProxyType,UnitsType>                     ProtoType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit StaticAdd() noexcept : ProtoType() {} //!< setup
                virtual ~StaticAdd() noexcept               {} //!< cleanup


            private:
                Y_DISABLE_COPY_AND_ASSIGN(StaticAdd);
            };
        }

    }

}

#endif
