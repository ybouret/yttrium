

#ifndef Y_Kemp_Element_Compute_Unary_Included
#define Y_Kemp_Element_Compute_Unary_Included 1

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
         - Element *Get(const Assembly<WORD> &)
         - Element *GetEx(const Assembly<WORD> &l, uint64_t &tmx)
         */
        //______________________________________________________________________
        template <template <typename,typename> class ENGINE,typename CORE, typename WORD>
        struct ComputeUnary
        {

            //__________________________________________________________________
            //
            //! Engine::Get with conversion
            //__________________________________________________________________
            static inline Element * Result(Element &arg) {
                return ENGINE<CORE,WORD>::Get( arg.get<WORD>()  );
            }

            //__________________________________________________________________
            //
            //! Engine::Get with conversion + timing
            //__________________________________________________________________
            static inline Element * ResTMX(Element &arg, uint64_t &tmx) {
                return ENGINE<CORE,WORD>::GetEx( arg.get<WORD>(),  tmx );
            }


        };

        //! declare functions for Unary ops
#define Y_Kemp_UnaryAPI(ENGINE,CORE,WORD) { \
ComputeUnary<ENGINE,CORE,WORD>::Result, \
ComputeUnary<ENGINE,CORE,WORD>::ResTMX, \
 }

        //! declare table of Unary ops
#define Y_Kemp_UnaryAPI_Table(ENGINE) {    \
Y_Kemp_UnaryAPI(ENGINE,uint64_t,uint32_t), \
Y_Kemp_UnaryAPI(ENGINE,uint64_t,uint16_t), \
Y_Kemp_UnaryAPI(ENGINE,uint64_t,uint8_t),  \
Y_Kemp_UnaryAPI(ENGINE,uint32_t,uint16_t), \
Y_Kemp_UnaryAPI(ENGINE,uint32_t,uint8_t),  \
Y_Kemp_UnaryAPI(ENGINE,uint16_t,uint8_t),  \
}


    }

}

#endif

