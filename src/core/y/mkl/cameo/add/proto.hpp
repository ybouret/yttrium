
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

            //__________________________________________________________________
            //
            //
            //
            //! Prototype addition for a TYPE, its PROXY and a selected STORAGE
            //
            //
            //__________________________________________________________________
            template <
            typename  T,
            typename  PROXY,
            typename  STORAGE>
            class AddProto : public STORAGE
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                Y_ARGS_DECL(T,Type);                       //!< aliases
                typedef typename PROXY::Unit     Unit;     //!< alias
                typedef typename PROXY::UnitArgs UnitArgs; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit AddProto() noexcept : STORAGE()             {} //!< setup empty
                explicit AddProto(size_t n)  : STORAGE(n,AsCapacity) {} //!< setup with capacity
                virtual ~AddProto() noexcept                         {} //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! insert a new iten
                inline void push(ParamType args)
                { UnitArgs u = args; this->insert(u); }

                //! insert a new item, helper
                inline AddProto & operator<<(ParamType args)
                { UnitArgs u = args; this->insert(u); return *this; }

                //! insert new items, helper
                inline void push(const size_t n, ParamType args)
                { UnitArgs u = args; this->insert(u,n); }

                //! call reduction algorithm
                inline Type sum()
                {
                    if(this->size()>0)
                        return PROXY::Policy::Reduce(*this);
                    else
                        return T(0);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(AddProto);
            };
        }
    }

}

#endif
