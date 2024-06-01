
#ifndef Y_Kemp_Element_Compute_Included
#define Y_Kemp_Element_Compute_Included 1

#include "y/kemp/element.hpp"
#include "y/system/wtime.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        //! high-level compute
        /**
         assuming thate ENGINE has static methods:
         - Element *Get(const Assembly<WORD> &l, const Assembly<WORD> &r)
         - Element *GetEx(const Assembly<WORD> &l, const Assembly<WORD> &r, uint64_t &tmx)
         */
        template <template <typename,typename> class ENGINE,typename CORE, typename WORD>
        struct Compute
        {

            static inline Element * Result(Element &lhs, Element &rhs) {
                return ENGINE<CORE,WORD>::Get( lhs.get<WORD>(), rhs.get<WORD>() );
            }

            static inline Element * ResTMX(Element &lhs, Element &rhs, uint64_t &tmx) {
                return ENGINE<CORE,WORD>::GetEx( lhs.get<WORD>(), rhs.get<WORD>(), tmx );
            }

            static inline Element *ResL64(uint64_t lhs, Element &rhs)
            {
                const Assembly<WORD> L(lhs);
                return ENGINE<CORE,WORD>::Get( lhs, rhs.get<WORD>() );
            }

            static inline Element * ResR64(Element  &lhs, uint64_t rhs)
            {
                const Assembly<WORD> R(rhs);
                return ENGINE<CORE,WORD>::Get( lhs.get<WORD>(), R );
            }
        };

#define Y_Kemp_BinaryAPI(ENGINE,CORE,WORD) { \
Compute<ENGINE,CORE,WORD>::Result, \
Compute<ENGINE,CORE,WORD>::ResTMX, \
Compute<ENGINE,CORE,WORD>::ResL64, \
Compute<ENGINE,CORE,WORD>::ResR64  \
}

#define Y_Kemp_BinaryAPI_Table(ENGINE) {    \
Y_Kemp_BinaryAPI(ENGINE,uint64_t,uint32_t), \
Y_Kemp_BinaryAPI(ENGINE,uint64_t,uint16_t), \
Y_Kemp_BinaryAPI(ENGINE,uint64_t,uint8_t),  \
Y_Kemp_BinaryAPI(ENGINE,uint32_t,uint16_t), \
Y_Kemp_BinaryAPI(ENGINE,uint32_t,uint8_t),  \
Y_Kemp_BinaryAPI(ENGINE,uint16_t,uint8_t),  \
}

#define Y_Kemp_TMX_Ini() const uint64_t ini64 = WallTime::Ticks()
#define Y_Kemp_TMX_Add() tmx += (WallTime::Ticks()-ini64)

    }

}

#endif

