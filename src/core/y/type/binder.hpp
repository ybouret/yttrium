//! \file


#ifndef Y_Type_Binder_Included
#define Y_Type_Binder_Included 1

#include "y/type/list.hpp"
#include "y/type/traits.hpp"
#include <iostream>

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! helper to declare parameters in Binder
    //
    //__________________________________________________________________________
#define Y_BINDER_DECL(I)                                               \
typedef typename TL::SafeTypeAt<TLIST,I-1,EmptyType>::Result Type##I;  \
typedef typename TypeTraits<Type##I>::ParamType              Param##I; \
typedef Int2Type<I>                                          ARGC##I;  \
Type##I                                                      arg##I

    //__________________________________________________________________________
    //
    //
    //! helper to implement derived class
    //
    //__________________________________________________________________________
#define Y_BINDER_ARGS(THE_LIST)                                                 \
typedef typename Binder<THE_LIST>::Param1 Param1; using Binder<THE_LIST>::arg1; \
typedef typename Binder<THE_LIST>::Param2 Param2; using Binder<THE_LIST>::arg2; \
typedef typename Binder<THE_LIST>::Param3 Param3; using Binder<THE_LIST>::arg3; \
typedef typename Binder<THE_LIST>::Param4 Param4; using Binder<THE_LIST>::arg4; \
typedef typename Binder<THE_LIST>::ArgsType ArgsType

    //__________________________________________________________________________
    //
    //
    //! Binding types
    //
    //__________________________________________________________________________
    template <typename TLIST>
    class Binder
    {
    public:
        //______________________________________________________________________
        //
        //
        // Declarations
        //
        //______________________________________________________________________
        Y_BINDER_DECL(1); //!< arg1
        Y_BINDER_DECL(2); //!< arg2
        Y_BINDER_DECL(3); //!< arg3
        Y_BINDER_DECL(4); //!< arg4
        static const size_t    ARGN = TL::LengthOf<TLIST>::Value; //!< alias
        typedef Int2Type<ARGN> ArgsType;                          //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! empty/default binder
        inline explicit Binder() : arg1(), arg2(), arg3(), arg4() {}

        //! setup1
        inline explicit Binder(Param1 p1) : arg1(p1), arg2(), arg3(), arg4() {}

        //! setup2
        inline explicit Binder(Param1 p1,
                               Param2 p2) : arg1(p1), arg2(p2), arg3(), arg4() {}

        //! setup3
        inline explicit Binder(Param1 p1,
                               Param2 p2,
                               Param3 p3) : arg1(p1), arg2(p2), arg3(p3), arg4() {}

        //! setup4
        inline explicit Binder(Param1 p1,
                               Param2 p2,
                               Param3 p3,
                               Param4 p4) : arg1(p1), arg2(p2), arg3(p3), arg4(p4) {}

        //! cleanup
        inline virtual ~Binder() noexcept {}

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const Binder &self)
        {
            os << '[';

            os << ' ' <<        self.arg1;
            os << ',' << ' ' << self.arg2;
            os << ',' << ' ' << self.arg3;
            os << ',' << ' ' << self.arg4;

            os << ' ' << ']';
            return os;
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Binder);
    };

}

#endif

