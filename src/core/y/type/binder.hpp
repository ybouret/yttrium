//! \file


#ifndef Y_Type_Binder_Included
#define Y_Type_Binder_Included 1

#include "y/type/list.hpp"
#include "y/type/traits.hpp"
#include <iostream>

namespace Yttrium
{

    namespace Core
    {
        template <const bool> struct InitBlock;

        //______________________________________________________________________
        //
        //
        //! Init Block/Primitive
        //
        //______________________________________________________________________
        template <> struct InitBlock<true>
        {
            //! zero block
            static void At(const void *, const size_t) noexcept;
        };

        //______________________________________________________________________
        //
        //
        //! Init Block/Compound
        //
        //______________________________________________________________________
        template <> struct InitBlock<false>
        {
            //! do nothing
            static void At(const void *, const size_t) noexcept;
        };

        //______________________________________________________________________
        //
        //
        //! Init Default Argument
        //
        //______________________________________________________________________
        template <typename T>
        inline void InitArgument(const T &arg)
        {
            typedef typename TypeTraits<T>::MutableType TT;
            InitBlock<TypeTraits<TT>::IsPrimitive>::At( &arg, sizeof(T) );
        }
    }
    //__________________________________________________________________________
    //
    //
    //! helper to declare parameters in Binder
    /**
     extract I-th argument, empty type if not in list
     */
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
    /**
     alias the arguments of the list
     */
    //
    //__________________________________________________________________________
#define Y_BINDER_ECHO(THE_LIST)                       \
typedef typename Binder<THE_LIST>::Param1   Param1;   \
typedef typename Binder<THE_LIST>::Param2   Param2;   \
typedef typename Binder<THE_LIST>::Param3   Param3;   \
typedef typename Binder<THE_LIST>::Param4   Param4;   \
typedef typename Binder<THE_LIST>::Param5   Param5;   \
typedef typename Binder<THE_LIST>::ArgsType ArgsType

    //__________________________________________________________________________
    //
    //
    //! helper to recall args
    //
    //__________________________________________________________________________
#define Y_BINDER_ARGS(THE_LIST) \
using Binder<THE_LIST>::arg1;   \
using Binder<THE_LIST>::arg2;   \
using Binder<THE_LIST>::arg3;   \
using Binder<THE_LIST>::arg4;   \
using Binder<THE_LIST>::arg5


    //__________________________________________________________________________
    //
    //
    //! Binding types, using Functor approach
    /**
     - extract types from the TLIST to build arg1,...,argN
     - use constructor with matching parameter types to setup
     */
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
        Y_BINDER_DECL(5); //!< arg5

        static const size_t    ARGN = TL::LengthOf<TLIST>::Value; //!< alias
        typedef Int2Type<ARGN> ArgsType;                          //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! empty/default binder
        inline explicit Binder() : 
        arg1(), arg2(), arg3(), arg4(), arg5() 
        {
            Core::InitArgument(arg1);
            Core::InitArgument(arg2);
            Core::InitArgument(arg3);
            Core::InitArgument(arg4);
            Core::InitArgument(arg5);
        }

        //! setup1
        inline explicit Binder(Param1 p1) : 
        arg1(p1), arg2(), arg3(), arg4(), arg5() 
        {
            Core::InitArgument(arg2);
            Core::InitArgument(arg3);
            Core::InitArgument(arg4);
            Core::InitArgument(arg5);
        }

        //! setup2
        inline explicit Binder(Param1 p1,
                               Param2 p2) :
        arg1(p1), arg2(p2), arg3(), arg4(), arg5() 
        {
            Core::InitArgument(arg3);
            Core::InitArgument(arg4);
            Core::InitArgument(arg5);
        }

        //! setup3
        inline explicit Binder(Param1 p1,
                               Param2 p2,
                               Param3 p3) : 
        arg1(p1), arg2(p2), arg3(p3), arg4(), arg5() 
        {
            Core::InitArgument(arg4);
            Core::InitArgument(arg5);
        }

        //! setup4
        inline explicit Binder(Param1 p1,
                               Param2 p2,
                               Param3 p3,
                               Param4 p4) : 
        arg1(p1), arg2(p2), arg3(p3), arg4(p4), arg5() 
        {
            Core::InitArgument(arg5);
        }


        //! setup5
        inline explicit Binder(Param1 p1,
                               Param2 p2,
                               Param3 p3,
                               Param4 p4,
                               Param5 p5) :
        arg1(p1), arg2(p2), arg3(p3), arg4(p4), arg5(p5) {}

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

