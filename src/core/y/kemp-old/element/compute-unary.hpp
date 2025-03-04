

#ifndef Y_Kemp_Element_Compute_Unary_Included
#define Y_Kemp_Element_Compute_Unary_Included 1

#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        //______________________________________________________________________
        //
        //
        //! high-level compute algorithms
        /**
         assuming thate ENGINE has static method:
         - Element *Get(const Assembly<WORD> &)
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

        };

        //! declare functions for Unary ops
#define Y_Kemp_UnaryAPI(ENGINE,CORE,WORD) { \
ComputeUnary<ENGINE,CORE,WORD>::Result    }

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

