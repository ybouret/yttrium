//! \file

#ifndef Y_Type_Args_Included
#define Y_Type_Args_Included 1

#include "y/type/traits.hpp"


//! declare internal types
#define Y_ARGS_EXPOSE(T,TYPE)                              \
typedef T                                   TYPE;          \
typedef typename TypeTraits<T>::MutableType Mutable##TYPE; \
typedef const Mutable##TYPE                 Const##TYPE

//! declare internal types and parameter type
#define Y_ARGS_DECL(T,TYPE)                           \
Y_ARGS_EXPOSE(T,TYPE);                                \
typedef typename TypeTraits<T>::ParamType Param##TYPE

#endif

