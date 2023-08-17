
//! \file

#ifndef Y_CamEO_Add_Proto_Included
#define Y_CamEO_Add_Proto_Included 1


#include "y/mkl/cameo/add/proxy.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Cameo
        {

            template <
            typename  T,
            typename  PROXY,
            typename  STORAGE>
            class AddProto : public STORAGE
            {
            public:
                Y_ARGS_DECL(T,Type);
                typedef typename PROXY::Unit     Unit;
                typedef typename PROXY::UnitArgs UnitArgs;

                explicit AddProto() noexcept : STORAGE() {}
                explicit AddProto(size_t n)  : STORAGE(n,AsCapacity) {}
                virtual ~AddProto() noexcept {}

                inline void push(ParamType args)
                {
                    UnitArgs u = args;
                    this->insert(u);
                }

                inline AddProto & operator<<(ParamType args)
                {
                    UnitArgs u = args;
                    this->insert(u);
                    return *this;
                }


                inline void push(const size_t n, ParamType args)
                {
                    UnitArgs u = args;
                    this->insert(u,n);
                }

                Type sum()
                {
                    if(this->size()>0)
                        return PROXY::Policy::Reduce(*this);
                    else
                        return 0;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(AddProto);
            };
        }
    }

}

#endif
