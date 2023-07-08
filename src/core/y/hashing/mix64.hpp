//! \file


#ifndef Y_Hashing_Mix64_Included
#define Y_Hashing_Mix64_Included 1

#include "y/config/starting.hpp"
#include "y/check/static.hpp"

namespace Yttrium
{
    namespace Hashing
    {
        //______________________________________________________________________
        //
        //
        //
        //! Mixing 64 bits integer
        //
        //
        //______________________________________________________________________
        class Mix64
        {
            
        protected:
            explicit Mix64() noexcept;

        public:
            virtual ~Mix64() noexcept;

            virtual const char * protocol()                         const noexcept = 0;
            virtual void         operator()(uint32_t &, uint32_t &) const noexcept = 0;

            template <typename T,typename U> inline
            U mix(const T value) const noexcept
            {
                Y_STATIC_CHECK(sizeof(T)<=sizeof(uint64_t),InputTooBig);
                Y_STATIC_CHECK(sizeof(U)<=sizeof(uint64_t),OutputTooBig);

                union {
                    uint32_t dw[2];
                    T        xx;
                    U        yy;
                } alias = { {0,0} };
                alias.xx = value;
                (*this)(alias.dw[0],alias.dw[1]);
                return alias.yy;
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mix64);
        };

    }
}

#endif

