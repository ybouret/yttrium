//! \file

#ifndef Y_Type_Args_Included
#define Y_Type_Args_Included 1

#include "y/type/traits.hpp"


//! declare internal types
#define Y_ARGS_EXPOSE(T)                                 \
typedef T                                   Type;        \
typedef typename TypeTraits<T>::MutableType MutableType; \
typedef const MutableType                   ConstType



#endif

