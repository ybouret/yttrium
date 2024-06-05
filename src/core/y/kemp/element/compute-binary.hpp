
#ifndef Y_Kemp_Element_Compute_Binary_Included
#define Y_Kemp_Element_Compute_Binary_Included 1

#include "y/kemp/element.hpp"
#include "y/kemp/element/tmx.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        //______________________________________________________________________
        //
        //
        //! high-level compute algorithms
        /**
         assuming thate ENGINE has static methods:
         - Element *Get(const Assembly<WORD> &l, const Assembly<WORD> &r)
         - Element *GetEx(const Assembly<WORD> &l, const Assembly<WORD> &r, uint64_t &tmx)
         */
        //______________________________________________________________________
        template <template <typename,typename> class ENGINE,typename CORE, typename WORD>
        struct ComputeBinary
        {

            //__________________________________________________________________
            //
            //! Engine::Get with conversion
            //__________________________________________________________________
            static inline Element * Result(Element &lhs, Element &rhs) {
                return ENGINE<CORE,WORD>::Get( lhs.get<WORD>(), rhs.get<WORD>() );
            }

            //__________________________________________________________________
            //
            //! Engine::Get with conversion + timing
            //__________________________________________________________________
            static inline Element * ResTMX(Element &lhs, Element &rhs, uint64_t &tmx) {
                return ENGINE<CORE,WORD>::GetEx( lhs.get<WORD>(), rhs.get<WORD>(), tmx );
            }

            //__________________________________________________________________
            //
            //! Engine::Get with uint64_t lhs
            //__________________________________________________________________
            static inline Element *ResL64(uint64_t lhs, Element &rhs)
            {
                const Assembly<WORD> L(lhs);
                return ENGINE<CORE,WORD>::Get( lhs, rhs.get<WORD>() );
            }

            //__________________________________________________________________
            //
            //! Engine::Get with uint64_t rhs
            //__________________________________________________________________
            static inline Element * ResR64(Element  &lhs, uint64_t rhs)
            {
                const Assembly<WORD> R(rhs);
                return ENGINE<CORE,WORD>::Get( lhs.get<WORD>(), R );
            }
        };

        //! declare functions for Binary ips
#define Y_Kemp_BinaryAPI(ENGINE,CORE,WORD) { \
ComputeBinary<ENGINE,CORE,WORD>::Result, \
ComputeBinary<ENGINE,CORE,WORD>::ResTMX, \
ComputeBinary<ENGINE,CORE,WORD>::ResL64, \
ComputeBinary<ENGINE,CORE,WORD>::ResR64  \
}

        //! declare table of Binary ops
#define Y_Kemp_BinaryAPI_Table(ENGINE) {    \
Y_Kemp_BinaryAPI(ENGINE,uint64_t,uint32_t), \
Y_Kemp_BinaryAPI(ENGINE,uint64_t,uint16_t), \
Y_Kemp_BinaryAPI(ENGINE,uint64_t,uint8_t),  \
Y_Kemp_BinaryAPI(ENGINE,uint32_t,uint16_t), \
Y_Kemp_BinaryAPI(ENGINE,uint32_t,uint8_t),  \
Y_Kemp_BinaryAPI(ENGINE,uint16_t,uint8_t),  \
}

      
    }

}

#endif

