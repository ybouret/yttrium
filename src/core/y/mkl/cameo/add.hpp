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

            //__________________________________________________________________
            //
            //
            //
            //! Add algorithms, flexible memory
            //
            //
            //__________________________________________________________________
            template <typename T, typename ALLOCATOR = Memory::Dyadic>
            class Add :
            public AddProto<T,Proxy<T>,typename Proxy<T>::template FlexibleUnits<ALLOCATOR>::Type>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Proxy<T>                                                    ProxyType; //!< alias
                typedef typename ProxyType::template FlexibleUnits<ALLOCATOR>::Type UnitsType; //!< alias
                typedef AddProto<T,ProxyType,UnitsType>                             ProtoType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Add()      noexcept : ProtoType()  {} //!< setup empty
                explicit Add(const size_t n) : ProtoType(n) {} //!< setup with capacity
                virtual ~Add()      noexcept                {} //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Add);
            };



        }

    }

}
#endif

